#!/usr/bin/env python3
import argparse
import json
import os
import re
import subprocess
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
FLAGS = [
    "-O2", "-mthumb", "-mthumb-interwork", "-mcpu=arm7tdmi",
    "-fno-builtin", "-nostdinc", "-ffreestanding", "-fcall-used-r4",
]
EXTERNAL = re.compile(r"(Func|Data|Value)_([0-9a-f]{8})")


def run(command):
    try:
        return subprocess.run(
            command, cwd=ROOT, check=True, text=True,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        )
    except subprocess.CalledProcessError as error:
        detail = (error.stderr or error.stdout or "").strip()
        raise RuntimeError(
            f"{Path(command[0]).name} failed: {detail}") from None


def compile_source(source, object_dir):
    stem = source.stem
    assembly = object_dir / f"{stem}.s"
    obj = object_dir / f"{stem}.o"
    run([
        str(ROOT / "toolchain/gcc296/xgcc"),
        f"-B{ROOT / 'toolchain/gcc296'}/", *FLAGS,
        "-S", "-o", str(assembly), str(source),
    ])
    run([
        "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
        "-o", str(obj), str(assembly),
    ])
    defined = [
        line.split()[-1] for line in
        run(["arm-none-eabi-nm", "-g", "--defined-only", str(obj)])
        .stdout.splitlines()
    ]
    expected = f"Func_{stem}"
    if defined != [expected]:
        raise ValueError(
            f"{source.name}: expected only {expected}, found {defined}")
    undefined = [
        line.split()[-1] for line in
        run(["arm-none-eabi-nm", "-u", str(obj)]).stdout.splitlines()
    ]
    for name in undefined:
        if EXTERNAL.fullmatch(name) is None:
            raise ValueError(f"{source.name}: unsupported external {name}")
    return obj, undefined


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument(
        "--jobs", type=int, default=min(16, os.cpu_count() or 1))
    parser.add_argument("--output", type=Path, default="out/claimed")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    sources = sorted((ROOT / "src").glob("*.c"))
    if not sources:
        raise ValueError("no reconstructed sources")
    addresses = [int(source.stem, 16) for source in sources]
    if len(addresses) != len(set(addresses)):
        raise ValueError("duplicate source address")
    if any(address < ROM_BASE or address >= ROM_BASE + len(rom)
           for address in addresses):
        raise ValueError("source address outside ROM")

    output = ROOT / args.output
    object_dir = output / "obj"
    object_dir.mkdir(parents=True, exist_ok=True)
    with ThreadPoolExecutor(max_workers=args.jobs) as executor:
        compiled = list(executor.map(
            lambda source: compile_source(source, object_dir), sources))

    objects = [item[0] for item in compiled]
    defined = {f"Func_{source.stem}" for source in sources}
    undefined = sorted({
        name for _, names in compiled for name in names if name not in defined
    })
    symbols_source = output / "externals.s"
    symbols_obj = output / "externals.o"
    symbols_source.write_text(
        ".syntax unified\n.thumb\n" +
        "".join(
            f".global {name}\n" +
            (".thumb_func\n" if name.startswith("Func_") else "") +
            f".set {name}, 0x{EXTERNAL.fullmatch(name).group(2)}\n"
            for name in undefined
        )
    )
    run([
        "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
        "-o", str(symbols_obj), str(symbols_source),
    ])

    linker = output / "claimed.ld"
    linker.write_text(
        "OUTPUT_ARCH(arm)\n"
        f"ENTRY(Func_{sources[0].stem})\n"
        "SECTIONS\n{\n" +
        "".join(
            f"  .func_{source.stem} 0x{source.stem} : "
            f"{{ {obj.relative_to(ROOT)}(.text) }}\n"
            for source, obj in zip(sources, objects)
        ) +
        "  /DISCARD/ : { *(.comment) *(.note*) }\n"
        "}\n"
    )
    elf = output / "claimed.elf"
    binary = output / "claimed.bin"
    run([
        "arm-none-eabi-ld", "-T", str(linker), "-o", str(elf),
        str(symbols_obj),
    ])
    run([
        "arm-none-eabi-objcopy", "-O", "binary",
        str(elf), str(binary),
    ])

    symbols = {}
    for line in run([
            "arm-none-eabi-nm", "-S", "--defined-only", str(elf),
    ]).stdout.splitlines():
        fields = line.split()
        if len(fields) == 4 and fields[3] in defined:
            symbols[fields[3]] = (int(fields[0], 16), int(fields[1], 16))
    if set(symbols) != defined:
        missing = sorted(defined - set(symbols))
        raise ValueError(f"missing linked functions: {missing}")

    image = binary.read_bytes()
    image_base = min(address for address, _ in symbols.values())
    manifest = []
    failures = []
    total = 0
    previous_end = 0
    for source in sources:
        name = f"Func_{source.stem}"
        address, size = symbols[name]
        if address != int(source.stem, 16):
            failures.append(f"{source.name}: linked at 0x{address:08x}")
            continue
        end = address + size
        if address < previous_end:
            failures.append(f"{source.name}: overlaps previous function")
        previous_end = max(previous_end, end)
        offset = address - image_base
        actual = image[offset:offset + size]
        expected = rom[address - ROM_BASE:end - ROM_BASE]
        if actual != expected:
            failures.append(f"{source.name}: linked bytes differ")
        total += size
        manifest.append({
            "source": str(source.relative_to(ROOT)),
            "symbol": name,
            "address": address,
            "size": size,
            "end": end,
        })
    (output / "manifest.json").write_text(json.dumps({
        "format": 1,
        "rom_base": ROM_BASE,
        "rom_size": len(rom),
        "image_base": image_base,
        "image_size": len(image),
        "claimed_bytes": total,
        "regions": manifest,
    }, indent=2) + "\n")

    print(
        f"linked={len(manifest)} failures={len(failures)} "
        f"claimed_bytes={total} image_bytes={len(image)}"
    )
    if failures:
        for failure in failures:
            print(failure)
        raise SystemExit(1)


if __name__ == "__main__":
    main()
