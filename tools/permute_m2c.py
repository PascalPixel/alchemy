#!/usr/bin/env python3
"""Close near-miss m2c drafts by nudging gcc296's register allocation.

Many m2c drafts compile to within a couple of bytes of the ROM: the code is
semantically right but the register allocator picks a different (equivalent)
assignment than the original compiler did. A targeted source mutation recovers
the ROM's choice without changing behaviour.

Two mutations are implemented, each of which lifts a subexpression into a named
temporary to nudge gcc296's allocator toward the original compiler's choice:

- state-block access ``M2C_FIELD((*(s32 *)ADDR + (VAR * K)), TYPE, OFF)``: the
  statement is wrapped in a block that lifts the base pointer and index; and
- address-like constants (0x02/0x03/0x08xxxxxx): each is lifted to a leading
  temporary.

Each candidate is compiled and byte-compared; a draft is accepted only when it
reproduces the ROM bytes exactly. This is a stopgap covering a couple of common
register-allocation idioms, not a general permuter (use decomp-permuter for the
rest); it exists so the matches it finds are reproducible.
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


ADDRESS = re.compile(r"0x0[238][0-9A-Fa-f]{6}")


def state_block(body, swap):
    def replace(match):
        indent, target, address, index, stride, kind, offset = match.groups()
        terms = ["(s32)_mo", "(s32)_mb"] if swap else ["(s32)_mb", "(s32)_mo"]
        total = " + ".join(terms + [f"(s32){offset}"])
        return (f"{indent}{{ s32 _mb = *(s32 *){address}; "
                f"s32 _mo = {index} * {stride}; {target}*({kind})({total}); }}")
    return STATEMENT.sub(replace, body)


def lift_constants(body):
    """Lift each address-like constant to a leading temporary."""
    opening = re.search(r"\)\s*\{", body)
    if not opening:
        return body
    cut = opening.end()
    constants = sorted(set(ADDRESS.findall(body)))
    declarations = "".join(
        f"\n    s32 _c{index} = {value};"
        for index, value in enumerate(constants))
    rest = body[cut:]
    for index, value in enumerate(constants):
        rest = rest.replace(value, f"_c{index}")
    return body[:cut] + declarations + rest


def candidates(body):
    yield state_block(body, False)
    yield state_block(body, True)
    yield lift_constants(body)


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
        hit = False
        for replacement in replacements:
            base = raw.replace("M2C_UNK", replacement) if replacement else raw
            for body in candidates(base):
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
                    hit = True
                    print(f"matched {stem}")
                    break
            if hit:
                break
    print(f"matched={matched} of {len(near)} near-misses")


if __name__ == "__main__":
    main()
