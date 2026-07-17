#!/usr/bin/env python3
"""Run call-graph discovery with extra thumb seeds for unreached code.

The default discovery only reaches code called from the reset vector, leaving
large regions (e.g. the 0x08029504 code body) undiscovered. Seeding the walk
with candidate prologue addresses lets it trace those functions with precise
control-flow boundaries (min_address == entry), which the raw prologue scan does
not give. Output matches discover.py's schema for export_code to consume.
"""
import argparse
import json
from pathlib import Path

from discover import Discovery, ROM_BASE

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument("-o", "--output", type=Path, required=True)
    parser.add_argument("--seeds", type=Path, required=True,
                        help="JSON discovery file whose entries seed the walk")
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    discovery = Discovery(rom)
    entry = discovery.initial_seeds()
    seed_document = json.loads(args.seeds.read_text())
    seed_functions = seed_document.get("functions", seed_document)
    seeded = 0
    for function in seed_functions:
        if function.get("mode") == "thumb":
            if discovery.add_seed(function["entry"], "thumb", "seeded"):
                seeded += 1
    discovery.run()
    report = discovery.report(entry)
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    print(f"seeded={seeded} functions={report['function_count']}")


if __name__ == "__main__":
    main()
