#!/usr/bin/env python3
"""Reconstruct a ROM code region (code interleaved with data) as v4t assembly.

Built on thumb_v4t (a strict ARMv4T decoder), so there is no objdump Thumb-2
ambiguity: a byte the decoder cannot read as a real ARM7TDMI instruction, or an
address the global control-flow walk never reached, is emitted as data. Branch
and call destinations become local labels when they land on an emitted
instruction in the region and `.set` symbols otherwise, so every reference
resolves and the region re-assembles byte-identically.
"""
import re
import struct
import subprocess
import tempfile
from pathlib import Path

import thumb_v4t

ROM_BASE = 0x08000000
HEX = re.compile(r"0x[0-9a-f]{8}")


ERRLINE = re.compile(r":(\d+): Error:")


def build_region(rom, start, end, instructions, jump_tables,
                 data=None, data_base=None, label="Region"):
    """Return assembly for [start, end) or raise if it is not byte-exact.

    instructions: {address: size} for every thumb instruction the walk reached;
    jump_tables: {table_address: [targets]} so the address lists are kept as
    data. By default addresses index into `rom` at ROM_BASE; pass data/data_base
    to disassemble a standalone buffer (e.g. a decompressed overlay at its EWRAM
    base). An instruction whose decode does not re-assemble to its exact bytes
    (a GAS canonicalization ambiguity) is demoted to a raw halfword, so the
    result is always byte-exact.
    """
    if data is None:
        data, data_base = rom, ROM_BASE
    expected = data[start - data_base:end - data_base]

    def u16(address):
        return struct.unpack_from("<H", data, address - data_base)[0]

    def u32(address):
        return struct.unpack_from("<I", data, address - data_base)[0]

    table_words = set()
    for table, targets in jump_tables.items():
        for index in range(len(targets)):
            word = table + 4 * index
            if start <= word < end:
                table_words.add(word)

    raw = set()

    def assemble(raw_forced):
        stream = []
        emitted = {}
        cursor = start
        while cursor < end:
            decoded = None
            if (cursor not in raw_forced and cursor not in table_words
                    and cursor in instructions):
                following = u16(cursor + 2) if cursor + 2 < end else None
                decoded = thumb_v4t.disassemble(u16(cursor), following, cursor)
                if not (decoded and decoded[1] == instructions[cursor]
                        and cursor + decoded[1] <= end):
                    decoded = None
            if decoded:
                text, width, target = decoded
                emitted[cursor] = width
                stream.append((cursor, "code", text, target))
                cursor += width
            elif cursor % 4 == 0 and cursor + 4 <= end and cursor + 2 not in instructions:
                stream.append((cursor, "data", f"\t.4byte 0x{u32(cursor):08x}", None))
                cursor += 4
            else:
                stream.append((cursor, "data", f"\t.2byte 0x{u16(cursor):04x}", None))
                cursor += 2

        labels = {}
        for address, kind, text, target in stream:
            if (kind == "code" and target is not None
                    and start <= target < end and target in emitted):
                labels.setdefault(target, f".L_{target:08x}")

        externals = {}
        lines = []
        row_owner = []
        for address, kind, text, target in stream:
            if address in labels:
                lines.append(f"{labels[address]}:")
                row_owner.append(None)
            if kind == "code" and target is not None:
                if target in labels:
                    symbol = labels[target]
                else:
                    symbol = f"sub_{target:08x}"
                    externals[target] = symbol
                text = HEX.sub(symbol, text)
            lines.append("\t" + text if kind == "code" else text)
            row_owner.append(address if kind == "code" else None)

        head = [".syntax unified", "\t.thumb"]
        for target, symbol in sorted(externals.items()):
            head.append(f"\t.set {symbol}, 0x{target:08x}")
        head += [f"\t.global {label}_{start:08x}", f"{label}_{start:08x}:"]
        source = "\n".join(head + lines) + "\n"
        return source, emitted, stream, row_owner, len(head)

    for _ in range(len(data)):
        source, emitted, stream, row_owner, header_lines = assemble(raw)
        try:
            built = assemble_region(source, start)
        except ValueError as error:
            match = ERRLINE.search(str(error))
            if not match:
                raise
            index = int(match.group(1)) - header_lines - 1
            owner = row_owner[index] if 0 <= index < len(row_owner) else None
            if owner is None or owner in raw:
                raise ValueError(f"region 0x{start:08x} cannot be reduced")
            raw.add(owner)
            continue
        if built == expected:
            return source, emitted
        difference = next(offset for offset in range(len(built))
                          if built[offset] != expected[offset])
        owner = None
        for address, kind, text, target in stream:
            if kind == "code" and address <= start + difference < address + emitted[address]:
                owner = address
                break
        if owner is None or owner in raw:
            raise ValueError(f"region 0x{start:08x} is not byte-exact")
        raw.add(owner)
    raise ValueError(f"region 0x{start:08x} did not converge")


def assemble_region(source, base):
    text = source.read_text() if isinstance(source, Path) else source
    with tempfile.TemporaryDirectory() as work:
        work = Path(work)
        (work / "r.s").write_text(text)
        assembled = subprocess.run(
            ["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
             "-o", str(work / "r.o"), str(work / "r.s")],
            capture_output=True, text=True)
        if assembled.returncode:
            raise ValueError(assembled.stderr.strip())
        if subprocess.run(["arm-none-eabi-ld", f"-Ttext=0x{base:08x}", "-o",
                           str(work / "r.elf"), str(work / "r.o")],
                          capture_output=True).returncode:
            raise ValueError("region link failed")
        subprocess.run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text",
                        str(work / "r.elf"), str(work / "r.bin")], check=True)
        return (work / "r.bin").read_bytes()
