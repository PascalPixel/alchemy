#!/usr/bin/env python3
"""Emit long-call interworking veneers as tracked reconstruction assembly.

A veneer is a fixed 8-byte thumb stub, `ldr r4, [pc, #0]; bx r4` followed by
its 4-byte absolute target, that the linker inserts so a bl can reach a distant
routine. gcc cannot emit a bare register tail-branch from C, so each verified
veneer is claimed under asm/. A candidate is claimed only when it is 4-aligned,
matches the stub shape exactly, carries a thumb-bit target in ROM, is a
discovered function boundary (work/functions-current.json), and is not already claimed.
build_asm.py re-verifies every emitted region against the private ROM.
"""
import argparse
import json
import struct
from pathlib import Path

from disassemble_function import disassemble

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
HEADER = ("@ 遠距離呼出し用モード間ベニア。固定のldr r4,[pc]; bx r4は\n"
          "@ リンカが遠距離処理へ到達するために生成する。Cでは表現不能。\n")


def claimed_spans():
    spans = []
    for name in ("out/claimed/manifest.json", "out/asm/manifest.json"):
        path = ROOT / name
        if path.exists():
            for region in json.loads(path.read_text())["regions"]:
                spans.append((region["address"],
                              region["address"] + region["size"]))
    return spans


def is_veneer(rom, address):
    if address % 4:
        return False
    offset = address - ROM_BASE
    word0, word1 = struct.unpack_from("<2H", rom, offset)
    reg = (word0 >> 8) & 7
    if (word0 & 0xf800) != 0x4800 or word0 & 0xff or word1 != (0x4700 | reg << 3):
        return False
    target = struct.unpack_from("<I", rom, offset + 4)[0]
    return bool(target & 1) and ROM_BASE <= (target & ~1) < ROM_BASE + len(rom)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("--functions", type=Path,
                        default=ROOT / "work/functions-current.json")
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    functions = json.loads(args.functions.read_text())
    functions = functions.get("functions", functions)
    spans = claimed_spans()

    def claimed(address):
        return any(low <= address < high for low, high in spans)

    asm_dir = ROOT / "asm"
    asm_dir.mkdir(exist_ok=True)
    emitted = 0
    for function in functions:
        address = function["entry"]
        if claimed(address) or not is_veneer(rom, address):
            continue
        data = rom[address - ROM_BASE:address - ROM_BASE + 8]
        listing = disassemble(data, address)
        (asm_dir / f"{address:08x}.s").write_text(HEADER + listing)
        emitted += 1
    print(f"veneers={emitted}")


if __name__ == "__main__":
    main()
