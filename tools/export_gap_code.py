#!/usr/bin/env python3
"""Reconstruct push-prologue functions in undiscovered code gaps.

The default call-graph discovery never reaches large code regions (overlays
copied to RAM, dispatch-table-only callees). This claims each function that
starts with a `push {..., lr}` prologue, sizing it to the next prologue so the
trailing literal pool is included, then requires it to disassemble as clean
thumb and re-assemble byte-identically. Anything that is data, overlaps a claim,
or fails to round-trip is skipped. build_asm.py re-verifies every region.
"""
import argparse
import json
import re
import struct
import subprocess
import tempfile
from bisect import bisect_right
from pathlib import Path

from disassemble_function import disassemble

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
HEADER = ("@ Reconstructed thumb disassembly of a code-gap function the call-graph\n"
          "@ discovery never reached. Verified byte-identical by build_asm.py.\n")


def number(value):
    return int(value, 0) if isinstance(value, str) else int(value)


def end_candidates(rom, entry, cap=0x2000):
    """Linear thumb decode from entry, yielding candidate function-end sizes.

    Each candidate is a position just past a return instruction plus the pool
    words the function references so far. Tracks pc-relative pool words as data
    and follows the 32-bit BL width. The caller round-trips candidates
    largest-first to pick the true end. Stops early if it hits a word that is
    neither a decodable position nor a tracked pool (a strong data signal).
    """
    off = entry - ROM_BASE
    pos = 0
    pools = set()
    candidates = []
    while pos < cap and off + pos + 1 < len(rom):
        address = entry + pos
        if address in pools:
            pos += 4
            continue
        word = struct.unpack_from("<H", rom, off + pos)[0]
        if (word & 0xf800) == 0x4800:
            target = ((address + 4) & ~3) + ((word & 0xff) << 2)
            if entry <= target < entry + cap:
                pools.add(target)
        if ((word & 0xf800) == 0xf000 and off + pos + 3 < len(rom)
                and (struct.unpack_from("<H", rom, off + pos + 2)[0] & 0xf800) == 0xf800):
            pos += 4
            continue
        is_return = (word == 0x4770 or (word & 0xff00) == 0xbd00
                     or ((word & 0xff87) == 0x4700 and (word >> 3) & 0xf in (12, 14)))
        pos += 2
        if is_return:
            end = pos
            while (entry + end) in pools:
                end += 4
            candidates.append(end)
    return sorted(set(candidates), reverse=True)[:8]


def claimed_spans():
    spans = []
    for name in ("out/claimed/manifest.json", "out/asm/manifest.json",
                 "out/assets/manifest.json"):
        path = ROOT / name
        if path.exists():
            for region in json.loads(path.read_text())["regions"]:
                spans.append((number(region["address"]),
                              number(region["address"]) + number(region["size"])))
    manifest = ROOT / "assets/manifest.json"
    if manifest.exists():
        for region in json.loads(manifest.read_text()).get("regions", []):
            spans.append((number(region["address"]),
                          number(region["address"]) + number(region["size"])))
    return spans


def round_trips(rom, address, size, listing):
    with tempfile.TemporaryDirectory() as work:
        work = Path(work)
        (work / "f.s").write_text(listing)
        obj = work / "f.o"
        if subprocess.run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
                           "-o", str(obj), str(work / "f.s")],
                          capture_output=True).returncode:
            return False
        undefined = subprocess.run(["arm-none-eabi-nm", "-u", str(obj)],
                                   capture_output=True, text=True).stdout
        names = [line.split()[-1] for line in undefined.splitlines() if line.split()]
        (work / "s.s").write_text(".syntax unified\n.thumb\n" + "".join(
            f".global {n}\n.thumb_func\n.set {n}, 0x{n.rsplit('_', 1)[1]}\n"
            for n in names if re.fullmatch(r"(Func|Data|Value)_[0-9a-f]{8}", n)))
        subprocess.run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
                        "-o", str(work / "s.o"), str(work / "s.s")], check=True)
        elf = work / "f.elf"
        if subprocess.run(["arm-none-eabi-ld", f"-Ttext=0x{address:08x}",
                           "-e", f"0x{address:08x}", "-o", str(elf),
                           str(obj), str(work / "s.o")],
                          capture_output=True).returncode:
            return False
        subprocess.run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text",
                        str(elf), str(work / "f.bin")], check=True)
        built = (work / "f.bin").read_bytes()
        return len(built) == size and built == rom[address - ROM_BASE:address - ROM_BASE + size]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("--functions", type=Path,
                        default=ROOT / "work/functions-all-prologues.json")
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    functions = json.loads(args.functions.read_text())
    functions = functions.get("functions", functions)
    spans = claimed_spans()

    def overlaps(low, high):
        return any(a < high and low < b for a, b in spans)

    prologues = sorted({f["entry"] for f in functions
                        if f.get("mode") == "thumb"
                        and (struct.unpack_from("<H", rom, f["entry"] - ROM_BASE)[0]
                             & 0xff00) == 0xb500})
    asm_dir = ROOT / "asm"
    asm_dir.mkdir(exist_ok=True)
    emitted = skipped = 0
    for index, entry in enumerate(prologues):
        if overlaps(entry, entry + 2):
            continue
        chosen = None
        for size in end_candidates(rom, entry):
            if size < 4 or overlaps(entry, entry + size):
                continue
            data = rom[entry - ROM_BASE:entry - ROM_BASE + size]
            try:
                listing = disassemble(data, entry)
            except Exception:
                continue
            if round_trips(rom, entry, size, listing):
                chosen = (size, listing)
                break
        if chosen is None:
            skipped += 1
            continue
        size, listing = chosen
        (asm_dir / f"{entry:08x}.s").write_text(HEADER + listing)
        spans.append((entry, entry + size))
        emitted += 1
    print(f"gap_functions={emitted} skipped={skipped}")


if __name__ == "__main__":
    main()
