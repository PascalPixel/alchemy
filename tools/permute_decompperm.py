#!/usr/bin/env python3
"""Close near-miss m2c drafts with decomp-permuter's randomizer.

permute_m2c.py handles a couple of register-allocation idioms by hand;
decomp-permuter's randomizer covers the rest (statement reordering, temp-for-
expression, associative-operand swaps, and so on). This drives that randomizer
with the repo's own byte-exact scorer (verify.verify) instead of decomp-
permuter's assembly-diff scorer, so a candidate is accepted only when it
re-produces the ROM bytes exactly, and each match is auto-committed: written to
src/<addr>.c and its asm/<addr>.s retired.

decomp-permuter is a user-supplied tool under toolchain/decomp-permuter (like
gcc296); clone github simonlindholm/decomp-permuter there and `pip install
pycparser toml`. Generic tooling, clean-room: it mutates the draft's own C, it
never introduces game knowledge.
"""
import argparse
import json
import random
import subprocess
import sys
from pathlib import Path

from verify import verify

ROOT = Path(__file__).resolve().parents[1]
PERMUTER = ROOT / "toolchain/decomp-permuter"
TYPES = Path(__file__).with_name("match_m2c.py").read_text().split(
    'TYPES = """\\\n')[1].split('"""')[0]


def load_permuter():
    sys.path.insert(0, str(PERMUTER))
    from src.candidate import Candidate
    from src.perm.perm import EvalState
    from src.helpers import get_default_randomization_weights
    return Candidate, EvalState, get_default_randomization_weights("gcc")


def preprocess(source):
    done = subprocess.run(["cpp", "-P", "-nostdinc"], input=source,
                          capture_output=True, text=True)
    return done.stdout if done.returncode == 0 else source


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("--drafts", type=Path, default=ROOT / "work/m2c-all")
    parser.add_argument("--report", type=Path,
                        default=ROOT / "work/matches/m2c.json")
    parser.add_argument("--tries", type=int, default=3000)
    parser.add_argument("--max-bytes", type=int, default=8)
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    output = ROOT / "out/permute"
    output.mkdir(parents=True, exist_ok=True)
    Candidate, EvalState, weights = load_permuter()

    tracked = {item.stem for item in (ROOT / "src").glob("*.c")}
    report = json.loads(args.report.read_text())
    rows = report if isinstance(report, list) else report.get("results", report)
    near = sorted((row for row in rows if not row.get("matched")
                   and row.get("mismatched_bytes", 999) <= args.max_bytes
                   and f"{row['entry']:08x}" not in tracked),
                  key=lambda row: row["mismatched_bytes"])

    matched = 0
    for row in near:
        stem = f"{row['entry']:08x}"
        draft = args.drafts / f"{stem}.c"
        if not draft.exists():
            continue
        raw = draft.read_text()
        replacements = (("s32", "u32", "void *", "s16")
                        if "M2C_UNK" in raw else (None,))
        found = None
        for replacement in replacements:
            body = raw.replace("M2C_UNK", replacement) if replacement else raw
            source = preprocess(TYPES + body)
            try:
                Candidate.from_source(source, EvalState(),
                                      f"Func_{stem}", weights, rng_seed=1)
            except Exception:
                break
            for _ in range(args.tries):
                candidate = Candidate.from_source(
                    source, EvalState(), f"Func_{stem}", weights,
                    rng_seed=random.randint(1, 10 ** 9))
                for _ in range(random.randint(1, 4)):
                    candidate.randomize_ast()
                text = candidate.get_source()
                probe = output / f"{stem}.c"
                probe.write_text(text)
                try:
                    ok, _ = verify(probe, rom, output)
                except Exception:
                    continue
                if ok:
                    found = text
                    break
            if found:
                break
        if found:
            (ROOT / "src" / f"{stem}.c").write_text(found)
            asm = ROOT / "asm" / f"{stem}.s"
            if asm.exists():
                asm.unlink()
            matched += 1
            print(f"matched {stem}")
    print(f"matched={matched} of {len(near)} near-misses")


if __name__ == "__main__":
    main()
