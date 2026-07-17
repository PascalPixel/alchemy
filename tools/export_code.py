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
import struct
from pathlib import Path

from disassemble_function import disassemble

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
HEADER = ("@ Reconstructed thumb disassembly of a discovered function. The ROM's\n"
          "@ compiler is not reproducible by free tools, so no asm-free C match\n"
          "@ exists yet; build_asm.py verifies these bytes against the ROM.\n")


def claimed_spans():
    spans = []
    for name in ("out/claimed/manifest.json", "out/asm/manifest.json"):
        path = ROOT / name
        if path.exists():
            for region in json.loads(path.read_text())["regions"]:
                spans.append((region["address"],
                              region["address"] + region["size"]))
    return spans


def returns(data):
    for offset in range(0, len(data) - 1, 2):
        word = struct.unpack_from("<H", data, offset)[0]
        if word == 0x4770 or (word & 0xff00) == 0xbd00:
            return True
        if (word & 0xff87) == 0x4700 and (word >> 3) & 0xf in (12, 14):
            return True
    return False


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("--functions", type=Path,
                        default=ROOT / "work/functions.json")
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    functions = json.loads(args.functions.read_text())
    functions = functions.get("functions", functions)
    spans = claimed_spans()

    def claimed(address):
        return any(low <= address < high for low, high in spans)

    asm_dir = ROOT / "asm"
    asm_dir.mkdir(exist_ok=True)
    emitted = skipped = 0
    for function in functions:
        address = function["entry"]
        size = function["max_address"] - address
        if (function.get("mode") != "thumb" or function.get("min_address") != address
                or size < 4 or size % 2 or claimed(address)
                or address - ROM_BASE + size > len(rom)):
            continue
        data = rom[address - ROM_BASE:address - ROM_BASE + size]
        if not returns(data):
            continue
        try:
            listing = disassemble(data, address)
        except Exception:
            skipped += 1
            continue
        (asm_dir / f"{address:08x}.s").write_text(HEADER + listing)
        emitted += 1
    print(f"code_functions={emitted} skipped={skipped}")


if __name__ == "__main__":
    main()
