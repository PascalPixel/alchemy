#!/usr/bin/env python3
"""Close near-miss m2c drafts by nudging gcc296's register allocation.

Many m2c drafts compile to within a couple of bytes of the ROM: the code is
semantically right but the register allocator picks a different (equivalent)
assignment than the original compiler did. A targeted source mutation recovers
the ROM's choice without changing behaviour.

The mutation implemented here handles the common state-block access
``M2C_FIELD((*(s32 *)ADDR + (VAR * K)), TYPE, OFF)``: the whole statement is
wrapped in a block that lifts the base pointer and the index into named
temporaries before the load, which makes gcc296 keep the pointer in the same
register the ROM used. Each candidate is compiled and byte-compared; a draft is
accepted only when it re-produces the ROM bytes exactly. This is a narrow
stopgap for one idiom, not a general permuter (use decomp-permuter for that);
it exists so the matches it finds are reproducible.
"""
import argparse
import json
import re
import subprocess
from pathlib import Path

from verify import verify

ROOT = Path(__file__).resolve().parents[1]
TYPES = Path(__file__).with_name("match_m2c.py").read_text().split(
    'TYPES = """\\\n')[1].split('"""')[0]

STATEMENT = re.compile(
    r"( *)((?:[\w\[\]().*>-]+ = )|(?:return ))"
    r"M2C_FIELD\(\(\*\(s32 \*\)(0x[0-9A-Fa-f]+) \+ \((\w+) \* (\d+)\)\), "
    r"(s32 \*|void \*\*|u32 \*|s16 \*|u8 \*|void \*), (0x[0-9A-Fa-f]+)\);")


def mutate(body, swap):
    def replace(match):
        indent, target, address, index, stride, kind, offset = match.groups()
        terms = [f"(s32)_mo", f"(s32)_mb"] if swap else [f"(s32)_mb", f"(s32)_mo"]
        total = " + ".join(terms + [f"(s32){offset}"])
        return (f"{indent}{{ s32 _mb = *(s32 *){address}; "
                f"s32 _mo = {index} * {stride}; {target}*({kind})({total}); }}")
    return STATEMENT.sub(replace, body)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("--drafts", type=Path, default=ROOT / "work/m2c-all")
    parser.add_argument("--report", type=Path,
                        default=ROOT / "work/matches/m2c.json")
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    output = ROOT / "out/permute"
    output.mkdir(parents=True, exist_ok=True)
    tracked = {item.stem for item in (ROOT / "src").glob("*.c")}
    report = json.loads(args.report.read_text())
    rows = report if isinstance(report, list) else report.get("results", report)
    near = sorted((row for row in rows if not row.get("matched")
                   and row.get("mismatched_bytes", 999) <= 4),
                  key=lambda row: row["mismatched_bytes"])

    matched = 0
    for row in near:
        stem = f"{row['entry']:08x}"
        if stem in tracked:
            continue
        draft = args.drafts / f"{stem}.c"
        if not draft.exists():
            continue
        raw = draft.read_text()
        replacements = (("s32", "u32", "void *", "s16", "u16", "s8")
                        if "M2C_UNK" in raw else (None,))
        for replacement in replacements:
            base = raw.replace("M2C_UNK", replacement) if replacement else raw
            for swap in (False, True):
                body = mutate(base, swap)
                if body == base:
                    continue
                candidate = output / f"{stem}.c"
                candidate.write_text(TYPES + body)
                try:
                    ok, _ = verify(candidate, rom, output)
                except (OSError, RuntimeError, ValueError, StopIteration,
                        subprocess.CalledProcessError):
                    continue
                if ok:
                    (ROOT / "src" / f"{stem}.c").write_text(TYPES + body)
                    asm = ROOT / "asm" / f"{stem}.s"
                    if asm.exists():
                        asm.unlink()
                    matched += 1
                    print(f"matched {stem}")
                    break
            else:
                continue
            break
    print(f"matched={matched} of {len(near)} near-misses")


if __name__ == "__main__":
    main()
