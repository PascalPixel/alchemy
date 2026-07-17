#!/usr/bin/env python3
"""Reconstruct discovered thumb functions as tracked disassembly.

The ROM was built with a compiler (ARM ADS/armcc) whose codegen free compilers
cannot reproduce for most functions, so these cannot yet be decompiled to
byte-matching C. Following the pret asm-first model, each confirmed code
function is claimed under asm/ as reconstructed disassembly until (if ever) an
asm-free C match is possible. Only functions that start cleanly and contain a
real return instruction are claimed, to avoid disassembling data as code.
build_asm.py re-verifies every region against the private ROM.
"""
import argparse
import json
import re
import struct
import subprocess
import tempfile
from pathlib import Path

from disassemble_function import disassemble

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
HEADER = ("@ Reconstructed thumb disassembly of a discovered function. The ROM's\n"
          "@ compiler is not reproducible by free tools, so no asm-free C match\n"
          "@ exists yet; build_asm.py verifies these bytes against the ROM.\n")


def number(value):
    return int(value, 0) if isinstance(value, str) else int(value)


def claimed_spans():
    spans = []
    for name in ("out/claimed/manifest.json", "out/asm/manifest.json",
                 "out/assets/manifest.json"):
        path = ROOT / name
        if path.exists():
            for region in json.loads(path.read_text())["regions"]:
                spans.append((number(region["address"]),
                              number(region["address"]) + number(region["size"])))
    # Never disassemble inside the asset/data region declared by the source
    # manifest, even before it is built: those bytes are data, not code.
    asset_manifest = ROOT / "assets/manifest.json"
    if asset_manifest.exists():
        document = json.loads(asset_manifest.read_text())
        for region in document.get("regions", []):
            spans.append((number(region["address"]),
                          number(region["address"]) + number(region["size"])))
    return spans


def returns(data):
    for offset in range(0, len(data) - 1, 2):
        word = struct.unpack_from("<H", data, offset)[0]
        if word == 0x4770 or (word & 0xff00) == 0xbd00:
            return True
        if (word & 0xff87) == 0x4700 and (word >> 3) & 0xf in (12, 14):
            return True
    return False


POOL_RANGE = re.compile(r"pool target 0x([0-9a-f]+) is out of range")


def disassemble_extending(rom, address, size, ceiling):
    """Disassemble, growing the region to cover trailing pool words it needs.

    The function discovery undercounts functions whose literal pool follows the
    body, so a pc-relative load can point past the recorded end. Grow to cover
    that word (never past `ceiling`, the next function/claim) and retry. Returns
    (listing, true_size).
    """
    for _ in range(16):
        data = rom[address - ROM_BASE:address - ROM_BASE + size]
        try:
            return disassemble(data, address), size
        except ValueError as error:
            match = POOL_RANGE.search(str(error))
            if not match:
                raise
            needed = int(match.group(1), 16) + 4 - address
            if needed <= size or address + needed > ceiling:
                raise
            size = needed
    raise ValueError("pool extension did not converge")


def round_trips(rom, address, size, listing):
    """Assemble a listing standalone and confirm it equals the ROM bytes.

    Guards against data mis-decoded as code (e.g. an ARMv5 bkpt objdump emits
    for a data halfword): such a listing fails to assemble or differs, and the
    function is skipped rather than written.
    """
    with tempfile.TemporaryDirectory() as work:
        work = Path(work)
        source = work / "f.s"
        source.write_text(listing)
        obj = work / "f.o"
        result = subprocess.run(
            ["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
             "-o", str(obj), str(source)],
            capture_output=True, text=True)
        if result.returncode:
            return False
        undefined = subprocess.run(
            ["arm-none-eabi-nm", "-u", str(obj)], capture_output=True, text=True).stdout
        symbols = work / "s.s"
        names = [line.split()[-1] for line in undefined.splitlines() if line.split()]
        symbols.write_text(".syntax unified\n.thumb\n" + "".join(
            f".global {name}\n.thumb_func\n.set {name}, 0x{name.rsplit('_', 1)[1]}\n"
            for name in names if re.fullmatch(r"(Func|Data|Value)_[0-9a-f]{8}", name)))
        symbol_obj = work / "s.o"
        subprocess.run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
                        "-o", str(symbol_obj), str(symbols)], check=True)
        elf = work / "f.elf"
        link = subprocess.run(
            ["arm-none-eabi-ld", f"-Ttext=0x{address:08x}", "-e", f"0x{address:08x}",
             "-o", str(elf), str(obj), str(symbol_obj)], capture_output=True, text=True)
        if link.returncode:
            return False
        binary = work / "f.bin"
        subprocess.run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text",
                        str(elf), str(binary)], check=True)
        built = binary.read_bytes()
        return built == rom[address - ROM_BASE:address - ROM_BASE + len(built)] \
            and len(built) == size


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("--functions", type=Path,
                        default=ROOT / "work/functions.json")
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    functions = json.loads(args.functions.read_text())
    functions = functions.get("functions", functions)
    starts = sorted({f["entry"] for f in functions} |
                    {high for _, high in claimed_spans()})
    spans = claimed_spans()

    def claimed(address):
        return any(low <= address < high for low, high in spans)

    def overlaps(low_bound, high_bound):
        return any(low < high_bound and low_bound < high for low, high in spans)

    def next_boundary(address):
        import bisect
        index = bisect.bisect_right(starts, address)
        return starts[index] if index < len(starts) else ROM_BASE + len(rom)

    asm_dir = ROOT / "asm"
    asm_dir.mkdir(exist_ok=True)
    emitted = skipped = 0
    for function in functions:
        address = function["entry"]
        if function.get("max_address") is None or function.get("min_address") != address:
            continue
        size = function["max_address"] - address
        if (function.get("mode") != "thumb"
                or size < 4 or size % 2 or claimed(address)
                or address - ROM_BASE + size > len(rom)):
            continue
        data = rom[address - ROM_BASE:address - ROM_BASE + size]
        if not returns(data):
            continue
        ceiling = min(next_boundary(address), ROM_BASE + len(rom))
        try:
            listing, true_size = disassemble_extending(rom, address, size, ceiling)
        except Exception:
            skipped += 1
            continue
        if overlaps(address, address + true_size) or not round_trips(
                rom, address, true_size, listing):
            skipped += 1
            continue
        (asm_dir / f"{address:08x}.s").write_text(HEADER + listing)
        emitted += 1
    print(f"code_functions={emitted} skipped={skipped}")


if __name__ == "__main__":
    main()
