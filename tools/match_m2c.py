#!/usr/bin/env python3
import argparse
import json
import os
import subprocess
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from verify import verify

ROOT = Path(__file__).resolve().parents[1]
TYPES = """\
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) \
    (*(type *)((u8 *)(base) + (offset)))

"""
REJECT = (
    "M2C_ERROR", "M2C_BITFIELD",
    "M2C_MEMSET", "M2C_MEMCPY",
)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument("drafts", type=Path)
    parser.add_argument("--jobs", type=int,
                        default=min(16, os.cpu_count() or 1))
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    candidate_dir = ROOT / "work/matches/m2c"
    output_dir = ROOT / "out/m2c"
    candidate_dir.mkdir(parents=True, exist_ok=True)
    output_dir.mkdir(parents=True, exist_ok=True)
    tracked = {item.name for item in (ROOT / "src").glob("*.c")}
    drafts = [
        item for item in sorted(args.drafts.glob("*.c"))
        if item.name not in tracked and
        not any(marker in item.read_text() for marker in REJECT)
    ]

    def match(draft):
        candidate = candidate_dir / draft.name
        function_output = output_dir / draft.stem
        function_output.mkdir(parents=True, exist_ok=True)
        source = draft.read_text()
        strategies = (
            (("s32", "u32") if "M2C_UNK" in source else (None,))
        )
        for replacement in strategies:
            body = (
                source.replace("M2C_UNK", replacement)
                if replacement is not None else source
            )
            candidate.write_text(TYPES + body)
            try:
                matched, size = verify(candidate, rom, function_output)
            except (
                OSError, RuntimeError, ValueError, StopIteration,
                subprocess.CalledProcessError,
            ):
                continue
            if matched:
                return {
                    "entry": int(draft.stem, 16),
                    "matched": True,
                    "size": size,
                    "source": str(candidate),
                    "unknown_type": replacement,
                }
        return {"entry": int(draft.stem, 16), "matched": False}

    with ThreadPoolExecutor(max_workers=args.jobs) as executor:
        results = list(executor.map(match, drafts))
    report = ROOT / "work/matches/m2c.json"
    report.write_text(json.dumps(results, indent=2) + "\n")
    matches = sum(item["matched"] for item in results)
    print(
        f"candidates={len(results)} matches={matches} "
        f"failures={len(results) - matches}"
    )


if __name__ == "__main__":
    main()
