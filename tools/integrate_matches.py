#!/usr/bin/env python3
"""Integrate permuter matches into src/, gated on exact region extent.

verify.py compares only the COMPILED FUNCTION LENGTH against the ROM, so a
draft whose function boundary is truncated passes even though it reproduces
just part of the ROM region. Integrating such a match silently detaches the
remainder (typically a literal pool) into ROM fallback -- a coverage
regression that looks like progress. This gate rejects any candidate whose
compiled .text size differs from the assembled size of the asm/ region it
would replace.
"""
import argparse
import re
import shutil
import subprocess
import sys
from pathlib import Path

from alchemy_gcc import CFLAGS, compiler_command

ROOT = Path(__file__).resolve().parents[1]
HELPER = re.compile(r"inline_fn|^(static|inline)\b", re.M)


def text_size(obj):
    listing = subprocess.run(["arm-none-eabi-objdump", "-h", str(obj)],
                             capture_output=True, text=True).stdout
    found = re.search(r"\.text\s+([0-9a-f]+)", listing)
    return int(found.group(1), 16) if found else None


def asm_extent(stem, scratch):
    """Size of the asm/<stem>.s region as currently committed."""
    source = ROOT / f"asm/{stem}.s"
    if not source.exists():
        return None
    listing = scratch / f"{stem}.asmprobe.s"
    shutil.copy(source, listing)
    obj = scratch / f"{stem}.asmprobe.o"
    done = subprocess.run(
        ["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
         "-o", str(obj), str(listing)], capture_output=True, text=True)
    return text_size(obj) if done.returncode == 0 else None


def c_extent(stem, candidate, scratch):
    """Size the candidate compiles to, using build_claimed's exact toolchain."""
    listing = scratch / f"{stem}.cprobe.s"
    obj = scratch / f"{stem}.cprobe.o"
    done = subprocess.run(
        compiler_command(*CFLAGS, "-S", "-o", listing, candidate),
        capture_output=True, text=True, cwd=ROOT)
    if done.returncode != 0:
        return None
    done = subprocess.run(
        ["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
         "-o", str(obj), str(listing)], capture_output=True, text=True)
    return text_size(obj) if done.returncode == 0 else None


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("directory", type=Path,
                        help="directory of src_<addr>.c permuter matches")
    parser.add_argument("--apply", action="store_true",
                        help="write accepted matches into src/ and drop asm/")
    args = parser.parse_args()
    scratch = args.directory / "gate"
    scratch.mkdir(exist_ok=True)

    accepted, rejected = [], []
    for candidate in sorted(args.directory.glob("src_*.c")):
        stem = candidate.stem.replace("src_", "")
        if (ROOT / f"src/{stem}.c").exists():
            continue
        if HELPER.search(candidate.read_text()):
            rejected.append((stem, "carries an m2c helper"))
            continue
        want = asm_extent(stem, scratch)
        got = c_extent(stem, candidate, scratch)
        if want is None or got is None:
            rejected.append((stem, f"could not measure (asm={want} c={got})"))
        elif want != got:
            rejected.append((stem, f"INCOMPLETE: asm={want}B c={got}B"))
        else:
            accepted.append((stem, want))

    for stem, size in accepted:
        print(f"accept {stem} ({size}B)")
        if args.apply:
            shutil.copy(args.directory / f"src_{stem}.c", ROOT / f"src/{stem}.c")
            (ROOT / f"asm/{stem}.s").unlink(missing_ok=True)
    for stem, why in rejected:
        print(f"reject {stem}: {why}")
    print(f"accepted={len(accepted)} rejected={len(rejected)}"
          f"{' (applied)' if args.apply else ' (dry run)'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
