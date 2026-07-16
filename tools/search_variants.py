#!/usr/bin/env python3
"""Search compiler-sensitive C declaration variants in parallel."""
import argparse
import hashlib
import json
import os
import re
import shutil
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from verify import ROM_BASE, verify


ROOT = Path(__file__).resolve().parents[1]
TYPES = ("s32", "u32", "s16", "u16", "s8", "u8", "void *")
DECLARATION = re.compile(
    r"(?m)^(?P<return>[A-Za-z_][A-Za-z0-9_ ]*?\*?)\s+"
    r"(?P<name>Func_[0-9a-f]{8})\((?P<params>[^;{}]*)\);$")
DEFINITION = re.compile(
    r"(?m)^(?P<return>[A-Za-z_][A-Za-z0-9_ ]*?\*?)\s+"
    r"(?P<name>Func_[0-9a-f]{8})\((?P<params>[^;{}]*)\)\s*\{")
LOCAL = re.compile(
    r"(?m)^(?P<indent>\s{4})(?P<type>s32|u32|s16|u16|s8|u8|void \*)"
    r"(?P<space>\s+)(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*;$")


def replace_once(old, new):
    return lambda source: source.replace(old, new, 1)


def declaration_operations(source):
    operations = []
    for match in DECLARATION.finditer(source):
        line = match.group(0)
        return_type = match.group("return").strip()
        for replacement in TYPES:
            if replacement != return_type:
                changed = line.replace(match.group("return"), replacement, 1)
                operations.append((f"{match.group('name')}:return={replacement}",
                                   replace_once(line, changed)))
        params = match.group("params").strip()
        if not params or params == "void":
            continue
        fields = [item.strip() for item in params.split(",")]
        for index, field in enumerate(fields):
            if field not in TYPES:
                continue
            for replacement in TYPES:
                if replacement == field:
                    continue
                changed_fields = list(fields)
                changed_fields[index] = replacement
                changed = line.replace(
                    match.group("params"), ", ".join(changed_fields), 1)
                operations.append((
                    f"{match.group('name')}:arg{index}={replacement}",
                    replace_once(line, changed),
                ))
    return operations


def definition_operations(source):
    operations = []
    match = DEFINITION.search(source)
    if match is None:
        return operations
    line = match.group(0)
    return_type = match.group("return").strip()
    for replacement in ("void", "s32", "u32", "s16", "u16"):
        if replacement != return_type:
            changed = line.replace(match.group("return"), replacement, 1)
            operations.append((f"definition:return={replacement}",
                               replace_once(line, changed)))
    params = match.group("params").strip()
    fields = [] if not params or params == "void" else [
        item.strip() for item in params.split(",")]
    for index, field in enumerate(fields):
        pieces = field.rsplit(" ", 1)
        if len(pieces) != 2 or pieces[0] not in TYPES:
            continue
        for replacement in TYPES:
            if replacement == pieces[0]:
                continue
            changed_fields = list(fields)
            changed_fields[index] = f"{replacement} {pieces[1]}"
            changed = line.replace(
                match.group("params"), ", ".join(changed_fields), 1)
            operations.append((f"definition:arg{index}={replacement}",
                               replace_once(line, changed)))
    for count in (1, 2):
        prefix = ", ".join(f"s32 unused{index}" for index in range(count))
        changed_params = prefix if not fields else prefix + ", " + params
        changed = line.replace(match.group("params"), changed_params, 1)
        operations.append((f"definition:prepend={count}",
                           replace_once(line, changed)))
    return operations


def local_operations(source):
    operations = []
    for match in LOCAL.finditer(source):
        line = match.group(0)
        changed = line.replace(match.group("type"),
                               "volatile " + match.group("type"), 1)
        operations.append((f"local:{match.group('name')}=volatile",
                           replace_once(line, changed)))
    return operations


def variants(source, maximum):
    operations = (declaration_operations(source) +
                  definition_operations(source) + local_operations(source))
    generated = [("baseline", source)]
    for label, operation in operations:
        generated.append((label, operation(source)))
    for left_index, (left_label, left) in enumerate(operations):
        for right_label, right in operations[left_index + 1:]:
            generated.append((left_label + "+" + right_label,
                              right(left(source))))
            if len(generated) >= maximum * 2:
                break
        if len(generated) >= maximum * 2:
            break
    unique = []
    seen = set()
    for label, body in generated:
        digest = hashlib.sha256(body.encode()).digest()
        if digest in seen:
            continue
        seen.add(digest)
        unique.append((label, body))
        if len(unique) >= maximum:
            break
    return unique


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument("candidates", type=Path)
    parser.add_argument("--spans", type=Path, required=True)
    parser.add_argument("--ranking", type=Path)
    parser.add_argument("--limit", type=int, default=20)
    parser.add_argument("--variants", type=int, default=128)
    parser.add_argument("--jobs", type=int,
                        default=min(18, os.cpu_count() or 1))
    parser.add_argument("--keep", type=int, default=5)
    parser.add_argument("-o", "--output", type=Path,
                        default=ROOT / "work/variants")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    spans = {
        item["entry"]: item["size"]
        for item in json.loads(args.spans.read_text())
    }
    tracked = {int(item.stem, 16) for item in (ROOT / "src").glob("*.c")}
    addresses = [
        int(item.stem, 16) for item in sorted(args.candidates.glob("*.c"))
        if int(item.stem, 16) in spans and int(item.stem, 16) not in tracked
    ]
    if args.ranking is not None:
        ranking = json.loads(args.ranking.read_text())
        scores = {
            item["entry"]: (
                item.get("mismatched_bytes", 1 << 30),
                -item.get("common_prefix", 0),
                item.get("size", 1 << 30),
            )
            for item in ranking if not item.get("matched", False)
        }
        addresses.sort(key=lambda address: scores.get(
            address, (1 << 30, 0, 1 << 30)))
    addresses = addresses[:args.limit]

    scratch = args.output / "scratch"
    matches = args.output / "matches"
    shutil.rmtree(scratch, ignore_errors=True)
    scratch.mkdir(parents=True, exist_ok=True)
    matches.mkdir(parents=True, exist_ok=True)
    tasks = []
    for address in addresses:
        source = (args.candidates / f"{address:08x}.c").read_text()
        for index, (label, body) in enumerate(variants(source, args.variants)):
            tasks.append((address, index, label, body))

    def compile_variant(task):
        address, index, label, body = task
        directory = scratch / f"{address:08x}" / f"{index:04d}"
        directory.mkdir(parents=True, exist_ok=True)
        candidate = directory / f"{address:08x}.c"
        candidate.write_text(body)
        try:
            actual, _, size = verify(candidate, rom, directory, details=True)
        except Exception as error:
            return {"entry": address, "variant": index, "label": label,
                    "error": type(error).__name__}
        span = spans[address]
        expected = rom[address - ROM_BASE:address - ROM_BASE + span]
        overlap = min(len(actual), len(expected))
        mismatch = sum(actual[i] != expected[i] for i in range(overlap))
        mismatch += abs(len(actual) - len(expected))
        prefix = 0
        for left, right in zip(actual, expected):
            if left != right:
                break
            prefix += 1
        return {
            "entry": address, "variant": index, "label": label,
            "size": size, "span": span, "mismatched_bytes": mismatch,
            "common_prefix": prefix,
            "matched": size == span and actual == expected,
            "source": str(candidate),
        }

    with ThreadPoolExecutor(max_workers=args.jobs) as executor:
        results = list(executor.map(compile_variant, tasks))
    grouped = {}
    for result in results:
        grouped.setdefault(result["entry"], []).append(result)
    report = []
    exact = 0
    for address in addresses:
        rows = [row for row in grouped.get(address, []) if "error" not in row]
        rows.sort(key=lambda row: (
            not row["matched"], row["mismatched_bytes"],
            -row["common_prefix"], abs(row["size"] - row["span"]),
            row["variant"],
        ))
        best = rows[:args.keep]
        if best and best[0]["matched"]:
            exact += 1
            destination = matches / f"{address:08x}.c"
            shutil.copyfile(best[0]["source"], destination)
            best[0]["match_source"] = str(destination)
        report.append({"entry": address, "tested": len(grouped.get(address, [])),
                       "best": best})
    args.output.mkdir(parents=True, exist_ok=True)
    (args.output / "report.json").write_text(
        json.dumps(report, indent=2) + "\n")
    print(f"targets={len(addresses)} variants={len(tasks)} exact={exact}")
    for item in report:
        if not item["best"]:
            continue
        best = item["best"][0]
        print(
            f"{item['entry']:08x} matched={best['matched']} "
            f"mismatch={best['mismatched_bytes']} "
            f"prefix={best['common_prefix']} label={best['label']}"
        )


if __name__ == "__main__":
    main()
