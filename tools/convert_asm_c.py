#!/usr/bin/env python3
"""Rewrite every C source that depends on inline asm as tracked assembly.

These functions only reached byte-identity through gcc register pins and asm
barriers, which are disallowed. Following the pret model, such fakematches move
out of src/ (genuine asm-free C only) into asm/ as reconstructed disassembly,
kept until an asm-free C match is found. build_asm.py re-verifies every region.
"""
import argparse
import json
from pathlib import Path

from build_claimed import ROM_BASE
from disassemble_function import disassemble

ROOT = Path(__file__).resolve().parents[1]
HEADER = ("@ Reconstructed thumb disassembly. This function reached byte-identity\n"
          "@ only through inline-asm register pins, which are disallowed in C, so\n"
          "@ it lives here until an asm-free C match is found. build_asm.py\n"
          "@ re-verifies it against the private ROM.\n")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("--claimed", type=Path,
                        default=ROOT / "out/claimed/manifest.json")
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    sizes = {region["address"]: region["size"]
             for region in json.loads(args.claimed.read_text())["regions"]}
    asm_dir = ROOT / "asm"
    asm_dir.mkdir(exist_ok=True)
    converted = []
    for source in sorted((ROOT / "src").glob("*.c")):
        if "asm" not in source.read_text():
            continue
        address = int(source.stem, 16)
        size = sizes.get(address)
        if size is None:
            raise ValueError(f"{source.name}: no verified size to disassemble")
        data = rom[address - ROM_BASE:address - ROM_BASE + size]
        listing = disassemble(data, address)
        (asm_dir / f"{source.stem}.s").write_text(HEADER + listing)
        source.unlink()
        converted.append(source.stem)
    print(f"converted={len(converted)}")


if __name__ == "__main__":
    main()
