#!/usr/bin/env python3
import argparse
import re
import subprocess
from pathlib import Path

from discover import Discovery, ROM_BASE, sx


def branch_target(data, address, kind):
    offset = address - ROM_BASE
    half = int.from_bytes(data[offset:offset + 2], "little")
    if kind == "branch":
        return address + 4 + (sx(half & 0x7FF, 11) << 1)
    if kind == "conditional":
        return address + 4 + (sx(half & 0xFF, 8) << 1)
    return None


def disassemble(data, start, end, instructions, output_dir):
    binary = output_dir / f"{start:08x}.bin"
    masked = bytearray(b"\xc0\x46" * ((end - start + 1) // 2))
    for address in instructions:
        size = instructions[address]["size"]
        source = address - ROM_BASE
        destination = address - start
        masked[destination:destination + size] = data[source:source + size]
    binary.write_bytes(masked[:end - start])
    text = subprocess.check_output([
        "arm-none-eabi-objdump", "-D", "-b", "binary", "-marm",
        "-Mforce-thumb", f"--adjust-vma=0x{start:08x}", str(binary),
    ], text=True)
    result = {}
    pattern = re.compile(
        r"^\s*([0-9a-f]+):\s+(?:[0-9a-f]{4}\s+)+(.*)$")
    for line in text.splitlines():
        match = pattern.match(line)
        if match:
            result[int(match.group(1), 16)] = match.group(2).strip()
    return result


def emit_discovery(rom, discovery, address, output):
    function = discovery.functions.get(address)
    if function is None:
        raise ValueError(f"unknown function: {address:08x}")
    if function["mode"] != "thumb":
        raise ValueError("only Thumb functions are currently supported")
    instructions = sorted(function["instructions"])
    start = instructions[0]
    end = max(
        item + discovery.instructions[item]["size"] for item in instructions)
    output_dir = output.parent
    output_dir.mkdir(parents=True, exist_ok=True)
    function_instructions = {
        item: discovery.instructions[item] for item in instructions
    }
    decoded = disassemble(
        rom, start, end, function_instructions, output_dir)
    calls = {source: target for source, target, mode in discovery.calls
             if source in function["instructions"] and mode == "thumb"}
    jump_tables = {
        table: discovery.jump_tables[table]
        for site, table in discovery.jump_table_sites.items()
        if site in function["instructions"]
    }
    labels = {
        target for item in instructions
        for target in [branch_target(
            rom, item, discovery.instructions[item]["kind"])]
        if target in function["instructions"]
    }
    labels.update(
        target for targets in jump_tables.values() for target in targets
        if target in function["instructions"]
    )
    literals = {}
    lines = [".syntax unified", ".thumb", f"glabel Func_{address:08x}"]
    for item in instructions:
        if item in labels:
            lines.append(f".L_{item:08x}:")
        instruction = decoded[item].split(";", 1)[0].strip()
        kind = discovery.instructions[item]["kind"]
        if kind == "call" and item in calls:
            mnemonic = instruction.split(None, 1)[0]
            instruction = f"{mnemonic} Func_{calls[item]:08x}"
        elif kind in ("branch", "conditional"):
            target = branch_target(rom, item, kind)
            if target in function["instructions"]:
                mnemonic = instruction.split(None, 1)[0]
                instruction = f"{mnemonic} .L_{target:08x}"
        half = discovery.u16(item)
        if half & 0xF800 == 0x4800:
            register = (half >> 8) & 7
            literal = ((item + 4) & ~3) + ((half & 0xFF) << 2)
            literals[literal] = discovery.u32(literal)
            instruction = f"ldr r{register}, .L_{literal:08x}"
        lines.append(f"    {instruction}")
    for literal, value in sorted(literals.items()):
        word = (
            f".L_{value:08x}"
            if value in jump_tables else f"0x{value:08x}"
        )
        lines.extend([
            "    .align 2",
            f".L_{literal:08x}:",
            f"    .word {word}",
        ])
    for table, targets in sorted(jump_tables.items()):
        lines.extend([
            "    .align 2",
            f".L_{table:08x}:",
            *(f"    .word .L_{target:08x}" for target in targets),
        ])
    output.write_text("\n".join(lines) + "\n")


def emit(rom, address, output):
    discovery = Discovery(rom)
    discovery.run()
    emit_discovery(rom, discovery, address, output)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument("address", type=lambda value: int(value, 0))
    parser.add_argument("-o", "--output", type=Path, required=True)
    args = parser.parse_args()
    emit(args.rom.read_bytes(), args.address & ~1, args.output)


if __name__ == "__main__":
    main()
