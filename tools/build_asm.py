#!/usr/bin/env python3
"""Assemble and byte-verify tracked assembly regions against the private ROM.

Each asm/<address>.s reconstructs one ROM region the approved C compiler
cannot emit (dispatch stubs, compiler-runtime thunks, BIOS-call wrappers) or
a function whose C draft does not yet match. Sources may reference
Func_/Data_/Value_ symbols, which resolve to their fixed addresses.
"""
import argparse
import json
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
EXTERNAL = re.compile(r"(Func|Data|Value)_([0-9a-f]{8})")


def run(command):
    try:
        return subprocess.run(
            command, cwd=ROOT, check=True, text=True,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except subprocess.CalledProcessError as error:
        detail = (error.stderr or error.stdout or "").strip()
        raise RuntimeError(f"{Path(command[0]).name} failed: {detail}") from None


def build_region(source, output_dir):
    address = int(source.stem, 16)
    obj = output_dir / f"{source.stem}.o"
    elf = output_dir / f"{source.stem}.elf"
    binary = output_dir / f"{source.stem}.bin"
    run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
         "-o", str(obj), str(source)])
    未定義 = run(["arm-none-eabi-nm", "-u", str(obj)]).stdout
    names = []
    for line in 未定義.splitlines():
        name = line.split()[-1]
        if not EXTERNAL.fullmatch(name):
            raise ValueError(f"{source.name}: unsupported external symbol {name}")
        names.append(name)
    objects = [str(obj)]
    if names:
        symbols_source = output_dir / f"{source.stem}.symbols.s"
        symbols_obj = output_dir / f"{source.stem}.symbols.o"
        symbols_source.write_text(".syntax unified\n.thumb\n" + "".join(
            f".global {name}\n" +
            (".thumb_func\n" if name.startswith("Func_") else "") +
            f".set {name}, 0x{name.rsplit('_', 1)[1]}\n"
            for name in names))
        run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
             "-o", str(symbols_obj), str(symbols_source)])
        objects.append(str(symbols_obj))
    run(["arm-none-eabi-ld", f"-Ttext=0x{address:08x}",
         "-e", f"0x{address:08x}", "-o", str(elf), *objects])
    run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text",
         str(elf), str(binary)])
    return address, binary.read_bytes()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("--output", type=Path, default="out/asm")
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    output = ROOT / args.output
    output.mkdir(parents=True, exist_ok=True)
    sources = sorted((ROOT / "asm").glob("*.s"))
    if not sources:
        raise ValueError("no reconstructed assembly sources")
    regions = []
    previous_end = 0
    for source in sources:
        address, data = build_region(source, output)
        if not ROM_BASE <= address < ROM_BASE + len(rom) or not data:
            raise ValueError(f"{source.name}: region outside ROM")
        if address < previous_end:
            raise ValueError(f"{source.name}: overlapping assembly region")
        previous_end = address + len(data)
        expected = rom[address - ROM_BASE:address - ROM_BASE + len(data)]
        if data != expected:
            raise ValueError(f"{source.name}: assembled bytes differ")
        regions.append({
            "address": address, "size": len(data),
            "source": str(source.relative_to(ROOT)),
            "output": str((output / f"{source.stem}.bin")),
        })
    (output / "manifest.json").write_text(json.dumps({
        "format": 1, "rom_base": ROM_BASE, "regions": regions,
    }, indent=2) + "\n")
    print(f"regions={len(regions)} "
          f"bytes={sum(region['size'] for region in regions)}")


if __name__ == "__main__":
    main()
