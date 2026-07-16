#!/usr/bin/env python3
import argparse
import re
from pathlib import Path

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

"""
DEFINITION = re.compile(
    r"(?m)^([A-Za-z_][A-Za-z0-9_ \t*]*?)"
    r"\s+(Func_[0-9a-f]{8})\s*\(([^)]*)\)\s*\{"
)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output", type=Path,
                        default=ROOT / "work/context.c")
    args = parser.parse_args()
    declarations = {}
    for source in sorted((ROOT / "src").glob("*.c")):
        text = source.read_text()
        for match in DEFINITION.finditer(text):
            return_type, name, parameters = match.groups()
            declarations[name] = (
                f"{' '.join(return_type.split())} "
                f"{name}({' '.join(parameters.split())});"
            )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(
        TYPES + "\n".join(declarations[name]
                          for name in sorted(declarations)) + "\n"
    )
    print(f"declarations={len(declarations)}")


if __name__ == "__main__":
    main()
