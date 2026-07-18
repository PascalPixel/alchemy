#!/usr/bin/env python3
import subprocess
import re
from pathlib import Path

from alchemy_gcc import CFLAGS, compiler_command

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
def run(command):
    subprocess.run(command, cwd=ROOT, check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def verify(source, rom, output_dir, details=False):
    address = int(source.stem, 16)
    symbol = f"Func_{address:08x}"
    assembly = output_dir / f"{source.stem}.s"
    obj = output_dir / f"{source.stem}.o"
    symbols_source = output_dir / f"{source.stem}.symbols.s"
    symbols_obj = output_dir / f"{source.stem}.symbols.o"
    elf = output_dir / f"{source.stem}.elf"
    binary = output_dir / f"{source.stem}.bin"
    run(compiler_command(
        *CFLAGS, "-S", "-o", assembly, source))
    run([
        "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
        "-o", str(obj), str(assembly),
    ])
    undefined = subprocess.check_output(
        ["arm-none-eabi-nm", "-u", str(obj)], text=True)
    names = []
    for line in undefined.splitlines():
        name = line.split()[-1]
        if not re.fullmatch(r"(Func|Data|Value)_[0-9a-f]{8}", name):
            raise ValueError(f"unsupported external symbol: {name}")
        names.append(name)
    symbols_source.write_text(
        ".syntax unified\n.thumb\n" +
        "".join(
            f".global {name}\n" +
            (".thumb_func\n" if name.startswith("Func_") else "") +
            f".set {name}, 0x{name.rsplit('_', 1)[1]}\n"
            for name in names
        )
    )
    run([
        "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
        "-o", str(symbols_obj), str(symbols_source),
    ])
    run([
        "arm-none-eabi-ld", f"-Ttext=0x{address:08x}", "-e", symbol,
        "-o", str(elf), str(obj), str(symbols_obj),
    ])
    run([
        "arm-none-eabi-objcopy", "-O", "binary", "-j", ".text",
        str(elf), str(binary),
    ])
    symbols = subprocess.check_output(
        ["arm-none-eabi-nm", "-S", str(elf)], text=True)
    row = next(line for line in symbols.splitlines()
               if line.endswith(" " + symbol))
    size = int(row.split()[1], 16)
    actual = binary.read_bytes()[:size]
    expected = rom[address - ROM_BASE:address - ROM_BASE + size]
    if details:
        return actual, expected, size
    return actual == expected, size


def main():
    rom = (ROOT / "baserom.gba").read_bytes()
    output_dir = ROOT / "out/verify"
    output_dir.mkdir(parents=True, exist_ok=True)
    sources = sorted((ROOT / "src").glob("*.c"))
    failures = []
    total = 0
    for source in sources:
        matched, size = verify(source, rom, output_dir)
        total += size
        if not matched:
            failures.append(source.name)
    print(f"verified={len(sources) - len(failures)} "
          f"failures={len(failures)} bytes={total}")
    if failures:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
