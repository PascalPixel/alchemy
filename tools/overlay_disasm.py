#!/usr/bin/env python3
"""Reconstruct a decompressed Thumb code overlay as symbol-referenced assembly.

Six compressed resources decompress to EWRAM-resident (base 0x02000000) Thumb
overlays: interworking dispatch stubs, functions, and pointer tables. They are
code, not graphics, so they are reconstructed as assembly rather than claimed as
assets.

The reachable code is found by a control-flow walk (discover.py's walker,
repointed at the overlay's EWRAM base) seeded from every push prologue and every
in-overlay dispatch-stub target. Bytes the walk reaches are disassembled to GAS
thumb with local labels for in-overlay branches and `.set` symbols for the calls
that leave the overlay; every byte the walk does not reach is emitted verbatim as
data. A byte-identical re-assembly is required, and any instruction whose
objdump form will not re-assemble is demoted to a raw halfword, so the result is
always byte-exact and never a blind dump of code as data.
"""
import re
import struct
import subprocess
import tempfile
from pathlib import Path

import discover

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
OVERLAY_BASE = 0x02000000

ROW = re.compile(r"\s*([0-9a-f]+):\t((?:[0-9a-f]{2,4} )+)\s*(\S.*)")
TARGET = re.compile(r"\b(b|bl|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|"
                    r"bgt|ble|bhs|blo)(\.[nw])?\s+0x([0-9a-f]+)\b")
ERRLINE = re.compile(r":(\d+): Error:")


def _objdump_rows(data, base):
    with tempfile.TemporaryDirectory() as work:
        binary = Path(work) / "overlay.bin"
        binary.write_bytes(data)
        text = subprocess.check_output([
            "arm-none-eabi-objdump", "-D", "-b", "binary", "-marmv4t",
            "-Mforce-thumb", f"--adjust-vma=0x{base:08x}", str(binary),
        ], text=True)
    rows = {}
    for line in text.splitlines():
        match = ROW.match(line)
        if match:
            width = 2 * len(match.group(2).split())
            rows[int(match.group(1), 16)] = (
                width, match.group(3).split(";")[0].strip())
    return rows


def _reachable(data, base):
    saved = discover.ROM_BASE
    discover.ROM_BASE = base
    try:
        discovery = discover.Discovery(data)
        for offset in range(0, len(data) - 1, 2):
            if (struct.unpack_from("<H", data, offset)[0] & 0xff00) == 0xb500:
                discovery.add_seed(base + offset, "thumb", "prologue")
        for offset in range(0, len(data) - 8, 2):
            word = struct.unpack_from("<H", data, offset)[0]
            following = struct.unpack_from("<H", data, offset + 2)[0]
            if 0x4800 <= word <= 0x48ff and 0x4700 <= following <= 0x47ff:
                target = struct.unpack_from("<I", data, offset + 4)[0]
                if base <= target < base + len(data):
                    discovery.add_seed(target & ~1, "thumb", "stub")
        for entry in list(discovery.functions):
            try:
                discovery.walk_function(entry)
            except Exception:
                pass
        instructions = {address: info["size"]
                        for address, info in discovery.instructions.items()
                        if base <= address < base + len(data)}
    finally:
        discover.ROM_BASE = saved
    return instructions


def assemble_overlay(source, base=OVERLAY_BASE):
    """Assemble reconstructed overlay assembly and return its raw bytes."""
    text = source.read_text() if isinstance(source, Path) else source
    with tempfile.TemporaryDirectory() as work:
        work = Path(work)
        (work / "o.s").write_text(text)
        assembled = subprocess.run(
            ["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
             "-o", str(work / "o.o"), str(work / "o.s")],
            capture_output=True, text=True)
        if assembled.returncode:
            raise ValueError(assembled.stderr.strip())
        linked = subprocess.run(
            ["arm-none-eabi-ld", f"-Ttext=0x{base:08x}",
             "-o", str(work / "o.elf"), str(work / "o.o")],
            capture_output=True, text=True)
        if linked.returncode:
            raise ValueError(linked.stderr.strip())
        subprocess.run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text",
                        str(work / "o.elf"), str(work / "o.bin")], check=True)
        return (work / "o.bin").read_bytes()


def build_overlay_source(decoded, base=OVERLAY_BASE):
    """Reconstruct overlay assembly that re-assembles byte-identically."""
    if len(decoded) % 2:
        raise ValueError("overlay has an odd byte length")
    rows = _objdump_rows(decoded, base)
    instructions = _reachable(decoded, base)
    covered = set()
    for address, size in instructions.items():
        covered.update(range(address, address + size))

    labels = {}
    for address in sorted(instructions):
        row = rows.get(address)
        if not row:
            continue
        found = TARGET.search(row[1])
        if found:
            target = int(found.group(3), 16)
            if target in instructions:
                labels.setdefault(target, f".L_{target:08x}")

    externals = {}

    def replacement(target):
        if target in labels:
            return labels[target]
        externals[target] = f"sub_{target:08x}"
        return externals[target]

    def retarget(mnemonic):
        found = TARGET.search(mnemonic)
        if not found:
            return mnemonic
        return mnemonic[:found.start(3) - 2] + replacement(int(found.group(3), 16))

    end = base + len(decoded)
    raw = set()

    def emit():
        body = []
        cursor = base
        while cursor < end:
            if cursor in labels:
                body.append((cursor, "label", f"{labels[cursor]}:"))
            row = rows.get(cursor)
            if (cursor not in raw and cursor in instructions and row
                    and all(byte in covered
                            for byte in range(cursor, cursor + row[0]))):
                body.append((cursor, "code", "\t" + retarget(row[1])))
                cursor += row[0]
                continue
            aligned = (cursor % 4 == 0 and cursor + 4 <= end
                       and not any(byte in covered
                                   for byte in range(cursor, cursor + 4))
                       and cursor + 4 not in instructions)
            if aligned:
                value = struct.unpack_from("<I", decoded, cursor - base)[0]
                body.append((cursor, "data", f"\t.4byte 0x{value:08x}"))
                cursor += 4
            else:
                value = struct.unpack_from("<H", decoded, cursor - base)[0]
                body.append((cursor, "data", f"\t.2byte 0x{value:04x}"))
                cursor += 2
        return body

    for _ in range(len(decoded)):
        externals = {}
        body = emit()
        head = [".syntax unified", "\t.thumb"]
        for target, name in sorted(externals.items()):
            head.append(f"\t.set {name}, 0x{target:08x}")
        head += [f"\t.global Overlay_{base:08x}", f"Overlay_{base:08x}:"]
        header_lines = len(head)
        text = "\n".join(head + [line for _, _, line in body]) + "\n"
        try:
            built = assemble_overlay(text, base)
        except ValueError as error:
            match = ERRLINE.search(str(error))
            if not match:
                raise
            index = int(match.group(1)) - header_lines - 1
            if not 0 <= index < len(body) or body[index][0] in raw:
                raise ValueError(f"cannot reconstruct near line {match.group(1)}")
            raw.add(body[index][0])
            continue
        if built == decoded:
            return text
        difference = next(offset for offset in range(len(built))
                          if built[offset] != decoded[offset])
        owner = base + difference
        forced = False
        for address, size in instructions.items():
            if address <= owner < address + size and address not in raw:
                raw.add(address)
                forced = True
                break
        if not forced:
            raise ValueError(f"irreducible mismatch at 0x{owner:08x}")
    raise ValueError("overlay reconstruction did not converge")
