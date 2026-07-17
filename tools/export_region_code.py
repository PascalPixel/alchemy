#!/usr/bin/env python3
"""Claim high-confidence code the per-function exporter could not bound.

export_cfg_code claims one function at a time and rejects any whose control-flow
walk over-merges or overlaps a neighbour. That leaves real, call-graph-reached
code stranded in the fallback. This claims those bytes as whole regions instead:
it takes the code the global walk reaches from the reset vector and the dispatch
table (high confidence, no speculative prologue seeding), carves the maximal
runs that are still unclaimed, and disassembles each with region_disasm's strict
ARMv4T decoder. Every region re-assembles byte-identically or it is dropped, and
build_asm re-verifies each one.
"""
import argparse
import json
from pathlib import Path

from discover import Discovery, ROM_BASE
from region_disasm import build_region

ROOT = Path(__file__).resolve().parents[1]
CODE_LIMIT = 0x08320000
HEADER = ("@ Reconstructed thumb disassembly of a call-graph-reached code region\n"
          "@ (code interleaved with its data), verified byte-identical by build_asm.\n")


def number(value):
    return int(value, 0) if isinstance(value, str) else int(value)


def claimed_mask(rom):
    mask = bytearray(len(rom))
    for name in ("out/claimed/manifest.json", "out/asm/manifest.json",
                 "out/assets/manifest.json", "assets/manifest.json"):
        path = ROOT / name
        if not path.exists():
            continue
        for region in json.loads(path.read_text()).get("regions", []):
            start = number(region["address"]) - ROM_BASE
            for index in range(start, start + number(region["size"])):
                if 0 <= index < len(mask):
                    mask[index] = 1
    return mask


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    args = parser.parse_args()
    rom = args.rom.read_bytes()

    discovery = Discovery(rom)
    discovery.run()
    code = bytearray(len(rom))
    for address, info in discovery.instructions.items():
        if info.get("mode") == "thumb" and address < CODE_LIMIT:
            for index in range(address - ROM_BASE, address - ROM_BASE + info["size"]):
                code[index] = 1
    instructions = {address: info["size"]
                    for address, info in discovery.instructions.items()
                    if info.get("mode") == "thumb"}
    jump_tables = discovery.jump_tables
    mask = claimed_mask(rom)

    # Instruction-start addresses that are wholly unclaimed, in order.
    starts = sorted(address for address in instructions
                    if address < CODE_LIMIT
                    and not any(mask[address - ROM_BASE + offset]
                                for offset in range(instructions[address])))
    asm_dir = ROOT / "asm"
    asm_dir.mkdir(exist_ok=True)
    emitted = dropped = claimed_bytes = 0
    index = 0
    while index < len(starts):
        start = starts[index]
        end = start + instructions[start]
        index += 1
        # Extend across adjacent unclaimed instructions and the small data gaps
        # between them, stopping at any claimed byte.
        while index < len(starts):
            nxt = starts[index]
            if nxt > end + 0x20 or any(mask[byte - ROM_BASE]
                                       for byte in range(end, nxt)):
                break
            end = nxt + instructions[nxt]
            index += 1
        if end - start < 0x40:
            dropped += 1
            continue
        try:
            source, decoded = build_region(rom, start, end, instructions, jump_tables)
        except ValueError:
            dropped += 1
            continue
        if sum(decoded.values()) * 100 < (end - start) * 60:
            dropped += 1
            continue
        (asm_dir / f"{start:08x}.s").write_text(HEADER + source)
        emitted += 1
        claimed_bytes += end - start
    print(f"regions={emitted} dropped={dropped} bytes={claimed_bytes}")


if __name__ == "__main__":
    main()
