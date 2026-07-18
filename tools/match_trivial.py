#!/usr/bin/env python3
import json
import re
import struct
import subprocess
from pathlib import Path

from alchemy_gcc import CFLAGS, compiler_command

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
def run(command):
    subprocess.run(command, cwd=ROOT, check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def candidate(function, rom):
    address = function["entry"]
    offset = address - ROM_BASE
    span = function["max_address"] - address
    if function["mode"] != "thumb" or function["min_address"] != address:
        return None
    first = struct.unpack_from("<H", rom, offset)[0]
    if span == 2 and first == 0x4770:
        return f"void Func_{address:08x}(void)\n{{\n}}\n"
    if span == 4:
        second = struct.unpack_from("<H", rom, offset + 2)[0]
        if first & 0xFF00 == 0x2000 and second == 0x4770:
            return (
                f"int Func_{address:08x}(void)\n{{\n"
                f"    return {first & 0xFF};\n}}\n"
            )
    if span == 6:
        half = struct.unpack_from("<3H", rom, offset)
        if (
            half[0] & 0xFF00 == 0x4B00 and
            half[1] == 0x7818 and
            half[2] == 0x4770
        ):
            literal_address = (
                (address + 4) & ~3) + ((half[0] & 0xFF) << 2)
            value = struct.unpack_from(
                "<I", rom, literal_address - ROM_BASE)[0]
            return (
                "typedef unsigned char u8;\n\n"
                f"u8 Func_{address:08x}(void)\n{{\n"
                f"    return *(u8 *)0x{value:08x};\n}}\n"
            )
    if span == 10:
        half = struct.unpack_from("<5H", rom, offset)
        load, left, right, scale, ret = half
        if (
            load & 0xF800 == 0x7800 and
            ((load >> 3) & 7) == 0 and (load & 7) == 0 and
            left & 0xF800 == 0 and
            ((left >> 6) & 0x1F) == 24 and (left & 0x3F) == 0 and
            right & 0xF800 == 0x1000 and
            ((right >> 6) & 0x1F) == 24 and (right & 0x3F) == 0 and
            scale & 0xF800 == 0 and (scale & 0x3F) == 0 and
            ret == 0x4770
        ):
            index = (load >> 6) & 0x1F
            shift = (scale >> 6) & 0x1F
            return (
                "typedef signed char s8;\n"
                "typedef unsigned char u8;\n"
                "typedef signed int s32;\n\n"
                f"s32 Func_{address:08x}(u8 *arg0)\n{{\n"
                f"    return (s8)arg0[{index}] << {shift};\n}}\n"
            )
        if (
            half[0] & 0xFF00 == 0x4800 and
            half[1] & 0xFF00 == 0x4B00 and
            half[2] == 0x1AC0 and
            half[3] & 0xF83F == 0x1000 and
            half[4] == 0x4770
        ):
            first_literal = (
                (address + 4) & ~3) + ((half[0] & 0xFF) << 2)
            second_literal = (
                (address + 6) & ~3) + ((half[1] & 0xFF) << 2)
            first_value = struct.unpack_from(
                "<I", rom, first_literal - ROM_BASE)[0]
            second_value = struct.unpack_from(
                "<I", rom, second_literal - ROM_BASE)[0]
            shift = (half[3] >> 6) & 0x1F
            return (
                "typedef unsigned char u8;\n"
                "typedef signed int s32;\n\n"
                f"extern u8 Data_{first_value:08x}[];\n"
                f"extern u8 Data_{second_value:08x}[];\n\n"
                f"s32 Func_{address:08x}(void)\n{{\n"
                f"    return (Data_{first_value:08x} - "
                f"Data_{second_value:08x}) >> {shift};\n"
                "}\n"
            )
    if span == 12 and len(function["callees"]) == 1:
        half = struct.unpack_from("<6H", rom, offset)
        if (
            half[0] == 0xB500 and
            half[1] & 0xFF00 == 0x4900 and
            half[2] & 0xF800 == 0xF000 and
            half[3] & 0xF800 == 0xF800 and
            half[4] == 0xBC01 and
            half[5] == 0x4700
        ):
            literal_address = (
                (address + 6) & ~3) + ((half[1] & 0xFF) << 2)
            value = struct.unpack_from(
                "<I", rom, literal_address - ROM_BASE)[0]
            callee = function["callees"][0]
            return (
                f"extern void Func_{callee:08x}(void *, void *);\n\n"
                f"void Func_{address:08x}(void *arg0)\n{{\n"
                f"    Func_{callee:08x}(arg0, (void *)0x{value:08x});\n}}\n"
            )
    if span == 12 and not function["callees"]:
        half = struct.unpack_from("<6H", rom, offset)
        if (
            half[0] & 0xFF00 == 0x4C00 and
            half[1] & 0xFF00 == 0x2100 and
            half[2] & 0xF83F == 0x0802 and
            half[3] == 0x400A and
            half[4] == 0x5110 and
            half[5] == 0x4770
        ):
            literal_address = (
                (address + 4) & ~3) + ((half[0] & 0xFF) << 2)
            value = struct.unpack_from(
                "<I", rom, literal_address - ROM_BASE)[0]
            shift = (half[2] >> 6) & 0x1F
            mask = half[1] & 0xFF
            return (
                "typedef unsigned int u32;\n"
                "typedef unsigned char u8;\n\n"
                f"void Func_{address:08x}(u32 arg0)\n{{\n"
                f"    register u32 base asm(\"r4\") = 0x{value:08x};\n"
                f"    register u32 mask asm(\"r1\") = {mask};\n"
                "    register u8 *address asm(\"r2\") =\n"
                f"        (u8 *)((arg0 >> {shift}) & mask);\n"
                "    *(u32 *)(address + base) = arg0;\n"
                "}\n"
            )
    if span == 20:
        half = struct.unpack_from("<10H", rom, offset)
        if (
            half[0] == 0xB500 and
            half[1] & 0xFF00 == 0x2800 and
            half[2] == 0xDD01 and
            half[3] & 0xFF00 == 0x2000 and
            half[4] == 0xE002 and
            half[5] == 0x2800 and
            half[6] == 0xDA00 and
            half[7] == 0x2000 and
            half[8] == 0xBC02 and
            half[9] == 0x4708 and
            (half[1] & 0xFF) == (half[3] & 0xFF)
        ):
            limit = half[1] & 0xFF
            return (
                "typedef signed int s32;\n\n"
                f"s32 Func_{address:08x}(s32 arg0)\n{{\n"
                f"    if (arg0 > {limit})\n"
                f"        return {limit};\n"
                "    if (arg0 < 0)\n"
                "        arg0 = 0;\n"
                "    return arg0;\n"
                "}\n"
            )
    if span == 22:
        half = struct.unpack_from("<11H", rom, offset)
        if (
            half[0] & 0xFF00 == 0x4900 and
            half[1] & 0xFF00 == 0x4B00 and
            half[2:8] == (0x680A, 0x1C10, 0x4358,
                          half[5], 0x18C0, 0x6008) and
            half[5] & 0xFF00 == 0x4B00 and
            half[8] & 0xF83F == 0x0000 and
            half[9] & 0xF83F == 0x0800 and
            half[10] == 0x4770
        ):
            literal_addresses = [
                ((address + index * 2 + 4) & ~3) +
                ((half[index] & 0xFF) << 2)
                for index in (0, 1, 5)
            ]
            values = [
                struct.unpack_from("<I", rom, item - ROM_BASE)[0]
                for item in literal_addresses
            ]
            left = (half[8] >> 6) & 0x1F
            right = (half[9] >> 6) & 0x1F
            return (
                "typedef signed int s32;\n"
                "typedef unsigned int u32;\n\n"
                f"u32 Func_{address:08x}(void)\n{{\n"
                "    s32 value;\n"
                f"    value = (*(s32 *)0x{values[0]:08x} * "
                f"0x{values[1]:08x}) + 0x{values[2]:x};\n"
                f"    *(s32 *)0x{values[0]:08x} = value;\n"
                f"    return (u32)(value << {left}) >> {right};\n"
                "}\n"
            )
    if span == 16:
        half = struct.unpack_from("<8H", rom, offset)
        if (
            half[0] == 0xB500 and
            half[1] & 0xFF00 == 0x2300 and
            half[2] & 0xF83F == 0x001B and
            half[3] == 0x4298 and
            half[4] == 0xDD00 and
            half[5] == 0x1C18 and
            half[6] == 0xBC02 and
            half[7] == 0x4708
        ):
            limit = (half[1] & 0xFF) << ((half[2] >> 6) & 0x1F)
            return (
                "typedef signed int s32;\n\n"
                f"s32 Func_{address:08x}(s32 arg0)\n{{\n"
                f"    if (arg0 > {limit})\n"
                f"        arg0 = {limit};\n"
                "    return arg0;\n"
                "}\n"
            )
    return None


def linked_text(obj, address, symbol, output_dir):
    未定義 = subprocess.check_output(
        ["arm-none-eabi-nm", "-u", str(obj)], text=True)
    names = []
    for line in 未定義.splitlines():
        name = line.split()[-1]
        if not re.fullmatch(r"(Func|Data|Value)_[0-9a-f]{8}", name):
            raise ValueError(f"unsupported external symbol: {name}")
        names.append(name)
    symbols_source = output_dir / f"{address:08x}.symbols.s"
    symbols_obj = output_dir / f"{address:08x}.symbols.o"
    elf = output_dir / f"{address:08x}.elf"
    binary = output_dir / f"{address:08x}.bin"
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
    return binary.read_bytes(), int(row.split()[1], 16)


def main():
    rom = (ROOT / "baserom.gba").read_bytes()
    manifest = json.loads((ROOT / "work/functions.json").read_text())
    source_dir = ROOT / "work/matches/trivial"
    output_dir = ROOT / "out/trivial"
    source_dir.mkdir(parents=True, exist_ok=True)
    output_dir.mkdir(parents=True, exist_ok=True)
    matches = []
    failures = []

    for function in manifest["functions"]:
        source = candidate(function, rom)
        if source is None:
            continue
        address = function["entry"]
        symbol = f"Func_{address:08x}"
        c_path = source_dir / f"{address:08x}.c"
        s_path = output_dir / f"{address:08x}.s"
        o_path = output_dir / f"{address:08x}.o"
        c_path.write_text(source)
        run(compiler_command(
            *CFLAGS, "-S", "-o", s_path, c_path))
        run([
            "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
            "-o", str(o_path), str(s_path),
        ])
        actual, size = linked_text(o_path, address, symbol, output_dir)
        actual = actual[:size]
        expected = rom[address - ROM_BASE:address - ROM_BASE + size]
        if actual == expected:
            matches.append({"entry": address, "size": size, "source": str(c_path)})
        else:
            failures.append(address)

    result = {"matches": matches, "failures": failures}
    (ROOT / "work/matches/trivial.json").write_text(
        json.dumps(result, indent=2) + "\n")
    print(f"matches={len(matches)} failures={len(failures)}")


if __name__ == "__main__":
    main()
