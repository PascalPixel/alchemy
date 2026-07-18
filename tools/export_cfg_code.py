#!/usr/bin/env python3
"""Reconstruct code-gap functions using precise control-flow-walk boundaries.

For each push-prologue candidate, walk its control flow (discover.py's
walk_function) to get the exact instruction extent. Keep only functions whose
walk starts at the entry (min == entry, i.e. no backward merge into a shared
region) and does not overlap a claim, then disassemble the code plus its literal
pool and require a byte-identical re-assembly. This captures multi-block
functions the linear boundary scan cannot bound, while refusing the merged /
ambiguous control-flow regions. build_asm.py re-verifies every region.
"""
import argparse
import json
import re
import struct
import subprocess
import tempfile
from pathlib import Path

from discover import Discovery, ROM_BASE
from disassemble_function import disassemble

ROOT = Path(__file__).resolve().parents[1]
HEADER = ("@ コード間隙関数の再構築サム逆アセンブル。範囲は\n"
          "@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。\n")
POOL_RANGE = re.compile(r"pool target 0x([0-9a-f]+) is out of range")


def number(value):
    return int(value, 0) if isinstance(value, str) else int(value)


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
        未定義 = subprocess.run(["arm-none-eabi-nm", "-u", str(obj)],
                                   capture_output=True, text=True).stdout
        names = [line.split()[-1] for line in 未定義.splitlines() if line.split()]
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


def disassemble_with_pool(rom, entry, code_end, ceiling, tables):
    size = code_end - entry
    while True:
        data = rom[entry - ROM_BASE:entry - ROM_BASE + size]
        words = [w for w in tables if entry <= w < entry + size]
        try:
            return disassemble(data, entry, data_words=words), size
        except ValueError as error:
            match = POOL_RANGE.search(str(error))
            if not match:
                return None, None
            needed = int(match.group(1), 16) + 4 - entry
            if needed <= size or entry + needed > ceiling:
                return None, None
            size = needed


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

    # コードはリソースポインタ表0x08320000より下にあり、それ以上は
    # リソースデータであり、サム命令に見えるバイト列は誤検出である。
    code_limit = 0x08320000
    prologues = sorted({f["entry"] for f in functions
                        if f.get("mode") == "thumb"
                        and f["entry"] < code_limit
                        and (struct.unpack_from("<H", rom, f["entry"] - ROM_BASE)[0]
                             & 0xff00) == 0xb500})
    boundaries = sorted(set(prologues) | {b for _, b in spans})
    discovery = Discovery(rom)
    asm_dir = ROOT / "asm"
    asm_dir.mkdir(exist_ok=True)
    emitted = skipped = 0
    claimed = set()
    for candidate in prologues:
        if overlaps(candidate, candidate + 2):
            continue
        # 真の関数先頭を求める。走査が開始候補より下へ到達した場合
        # （関数途中の偽プロローグ）なら、その最小位置が
        # PUSHプロローグなら、そこから関数全体を取り直す。
        entry = candidate
        for _ in range(8):
            discovery.add_seed(entry, "thumb", "cfg")
            try:
                discovery.walk_function(entry)
            except Exception:
                entry = None
                break
            insns = sorted(discovery.functions[entry]["instructions"])
            if not insns:
                entry = None
                break
            low = insns[0]
            if low == entry:
                break
            if (struct.unpack_from("<H", rom, low - ROM_BASE)[0] & 0xff00) != 0xb500:
                entry = None
                break
            entry = low
        if entry is None or entry in claimed:
            skipped += 1
            continue
        insns = sorted(discovery.functions[entry]["instructions"])
        if not insns or insns[0] != entry:
            skipped += 1
            continue
        code_end = insns[-1] + discovery.instructions[insns[-1]]["size"]
        # 関数内の分岐表ワードはデータ（アドレス列）なので、
        # アドレスを命令として解釈せず.4byteで出力する。
        tables = set()
        for table, targets in discovery.jump_tables.items():
            if entry <= table < code_end + 0x400:
                tables.update(table + 4 * i for i in range(len(targets)))
        from bisect import bisect_right
        ceiling = boundaries[bisect_right(boundaries, entry)] if bisect_right(
            boundaries, entry) < len(boundaries) else ROM_BASE + len(rom)
        listing, size = disassemble_with_pool(rom, entry, code_end,
                                              min(ceiling, entry + 0x2000), tables)
        if listing is None or overlaps(entry, entry + size):
            skipped += 1
            continue
        if not round_trips(rom, entry, size, listing):
            skipped += 1
            continue
        (asm_dir / f"{entry:08x}.s").write_text(HEADER + listing)
        spans.append((entry, entry + size))
        claimed.add(entry)
        emitted += 1
    print(f"cfg_functions={emitted} skipped={skipped}")


if __name__ == "__main__":
    main()
