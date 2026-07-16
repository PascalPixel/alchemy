#!/usr/bin/env python3
import argparse
import json
import os
import subprocess
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from discover import Discovery
from emit_function import emit_discovery

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument("--max-bytes", type=int, default=64)
    parser.add_argument("--limit", type=int)
    parser.add_argument("--jobs", type=int,
                        default=min(16, os.cpu_count() or 1))
    parser.add_argument("--context", type=Path)
    parser.add_argument("-o", "--output", type=Path,
                        default=ROOT / "work/m2c")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    discovery = Discovery(rom)
    discovery.run()
    args.output.mkdir(parents=True, exist_ok=True)
    tracked = {int(item.stem, 16) for item in (ROOT / "src").glob("*.c")}
    candidates = []
    for address, function in sorted(discovery.functions.items()):
        if (
            address in tracked or
            function["mode"] != "thumb" or
            function["unresolved"] or
            not function["instructions"]
        ):
            continue
        start = min(function["instructions"])
        end = max(
            item + discovery.instructions[item]["size"]
            for item in function["instructions"]
        )
        if start == address and end - start <= args.max_bytes:
            candidates.append((address, end - start))
    if args.limit is not None:
        candidates = candidates[:args.limit]

    m2c = ROOT / "toolchain/m2c-venv/bin/m2c"

    def decompile(candidate):
        address, size = candidate
        assembly = args.output / f"{address:08x}.s"
        source = args.output / f"{address:08x}.c"
        try:
            emit_discovery(rom, discovery, address, assembly)
            command = [
                str(m2c), "-t", "gba", "--valid-syntax",
                "--comment-style", "none", str(assembly),
            ]
            if args.context is not None:
                command[1:1] = [
                    "--context", str(args.context), "--no-cache",
                ]
            process = subprocess.run(
                command, text=True, capture_output=True)
            success = process.returncode == 0
        except (OSError, KeyError, ValueError):
            success = False
        if success:
            source.write_text(process.stdout)
        return {
            "entry": address,
            "size": size,
            "success": success,
        }

    with ThreadPoolExecutor(max_workers=args.jobs) as executor:
        results = list(executor.map(decompile, candidates))
    report = args.output / "report.json"
    report.write_text(json.dumps(results, indent=2) + "\n")
    success = sum(item["success"] for item in results)
    print(
        f"candidates={len(results)} success={success} "
        f"failures={len(results) - success}"
    )


if __name__ == "__main__":
    main()
