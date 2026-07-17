#!/usr/bin/env python3
"""Reconstruct a ROM code region (code interleaved with data) as v4t assembly.

Built on thumb_v4t (a strict ARMv4T decoder), so there is no objdump Thumb-2
ambiguity: a byte the decoder cannot read as a real ARM7TDMI instruction, or an
address the global control-flow walk never reached, is emitted as data. Branch
and call destinations become local labels when they land on an emitted
instruction in the region and `.set` symbols otherwise, so every reference
resolves and the region re-assembles byte-identically.
"""
import re
import struct
import subprocess
import tempfile
from pathlib import Path

import thumb_v4t

ROM_BASE = 0x08000000
HEX = re.compile(r"0x[0-9a-f]{8}")


def build_region(rom, start, end, instructions, jump_tables):
    """Return assembly for [start, end) or raise if it is not byte-exact.

    instructions: {address: size} for every thumb instruction the global walk
    reached; jump_tables: {table_address: [targets]} so the address lists are
    kept as data rather than decoded.
    """
    def u16(address):
        return struct.unpack_from("<H", rom, address - ROM_BASE)[0]

    def u32(address):
        return struct.unpack_from("<I", rom, address - ROM_BASE)[0]

    table_words = set()
    for table, targets in jump_tables.items():
        for index in range(len(targets)):
            word = table + 4 * index
            if start <= word < end:
                table_words.add(word)

    stream = []
    emitted = {}
    cursor = start
    while cursor < end:
        decoded = None
        if cursor not in table_words and cursor in instructions:
            following = u16(cursor + 2) if cursor + 2 < end else None
            decoded = thumb_v4t.disassemble(u16(cursor), following, cursor)
            if not (decoded and decoded[1] == instructions[cursor]
                    and cursor + decoded[1] <= end):
                decoded = None
        if decoded:
            text, width, target = decoded
            emitted[cursor] = width
            stream.append((cursor, "code", text, target))
            cursor += width
        elif cursor % 4 == 0 and cursor + 4 <= end and cursor + 2 not in instructions:
            stream.append((cursor, "data", f"\t.4byte 0x{u32(cursor):08x}", None))
            cursor += 4
        else:
            stream.append((cursor, "data", f"\t.2byte 0x{u16(cursor):04x}", None))
            cursor += 2

    labels = {}
    for address, kind, text, target in stream:
        if (kind == "code" and target is not None
                and start <= target < end and target in emitted):
            labels.setdefault(target, f".L_{target:08x}")

    externals = {}
    lines = []
    for address, kind, text, target in stream:
        if address in labels:
            lines.append(f"{labels[address]}:")
        if kind == "code" and target is not None:
            if target in labels:
                symbol = labels[target]
            else:
                symbol = f"sub_{target:08x}"
                externals[target] = symbol
            text = HEX.sub(symbol, text)
        lines.append("\t" + text if kind == "code" else text)

    head = [".syntax unified", "\t.thumb"]
    for target, symbol in sorted(externals.items()):
        head.append(f"\t.set {symbol}, 0x{target:08x}")
    head += [f"\t.global Region_{start:08x}", f"Region_{start:08x}:"]
    source = "\n".join(head + lines) + "\n"

    if assemble_region(source, start) != rom[start - ROM_BASE:end - ROM_BASE]:
        raise ValueError(f"region 0x{start:08x} is not byte-exact")
    return source, emitted


def assemble_region(source, base):
    text = source.read_text() if isinstance(source, Path) else source
    with tempfile.TemporaryDirectory() as work:
        work = Path(work)
        (work / "r.s").write_text(text)
        if subprocess.run(["arm-none-eabi-as", "-mcpu=arm7tdmi",
                           "-mthumb-interwork", "-o", str(work / "r.o"),
                           str(work / "r.s")], capture_output=True).returncode:
            raise ValueError("region assembly failed")
        if subprocess.run(["arm-none-eabi-ld", f"-Ttext=0x{base:08x}", "-o",
                           str(work / "r.elf"), str(work / "r.o")],
                          capture_output=True).returncode:
            raise ValueError("region link failed")
        subprocess.run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text",
                        str(work / "r.elf"), str(work / "r.bin")], check=True)
        return (work / "r.bin").read_bytes()
