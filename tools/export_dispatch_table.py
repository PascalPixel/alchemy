#!/usr/bin/env python3
"""Export the ROM-start dispatch-stub table as an assembly source.

Each entry is a fixed thumb stub -- ldr r4, [pc]; bx r4; .4byte target --
giving overlay code stable entry points into ROM routines. The table is not
compiler output: alchemy-gcc cannot emit a bare register tail-jump from C.
"""
import argparse
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
TABLE = 0x080000c0


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    offset = TABLE - ROM_BASE
    targets = []
    while True:
        words = struct.unpack_from("<2H", rom, offset)
        if words != (0x4c00, 0x4720):
            break
        targets.append(struct.unpack_from("<I", rom, offset + 4)[0])
        offset += 8
    if any(not target & 1 for target in targets):
        raise ValueError("dispatch target is missing its thumb bit")
    lines = [
        "@ ROM-start dispatch-stub table: stable thumb entry points into",
        "@ ROM routines for code loaded elsewhere. Regenerate with",
        "@ python3 tools/export_dispatch_table.py baserom.gba",
        "\t.thumb",
        "",
        "\t.macro dispatch target",
        "\tldr r4, 1f",
        "\tbx r4",
        "1:\t.4byte \\target",
        "\t.endm",
        "",
    ]
    lines.extend(f"\tdispatch Func_{target - 1:08x} + 1" for target in targets)
    (ROOT / "asm" / f"{TABLE:08x}.s").write_text("\n".join(lines) + "\n")
    print(f"entries={len(targets)} end=0x{TABLE + 8 * len(targets):08x}")


if __name__ == "__main__":
    main()
