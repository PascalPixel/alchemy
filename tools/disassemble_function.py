#!/usr/bin/env python3
"""Reconstruct a thumb function as a symbol-referenced assembly source.

The disassembler separates code from trailing literal pools by tracing every
pc-relative load, emits GAS-compatible thumb mnemonics with symbolic branch and
call targets, local labels for in-function jumps, and each pool word as a bare
constant. The result is meaningful reconstruction, not a byte dump; build_asm.py
re-assembles it and checks it is byte-identical to the private ROM before it may
be tracked.
"""
import re
import struct
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
ROW = re.compile(r"\s*([0-9a-f]+):\t((?:[0-9a-f]{2,4} )+)\s*(\S.*)")
LDR_PC = re.compile(r"\bldr\s+r\d+,\s*\[pc,\s*#(\d+)\]")
TARGET = re.compile(r"\b(b|bl|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|"
                    r"blt|bgt|ble|bhs|blo)(\.[nw])?\s+0x([0-9a-f]+)\b")


def objdump(data, address):
    binary = ROOT / "out/asm/scratch.bin"
    binary.parent.mkdir(parents=True, exist_ok=True)
    binary.write_bytes(data)
    text = subprocess.check_output([
        "arm-none-eabi-objdump", "-D", "-b", "binary", "-marmv4t",
        "-Mforce-thumb", f"--adjust-vma=0x{address:08x}", str(binary),
    ], text=True)
    rows = {}
    for line in text.splitlines():
        match = ROW.match(line)
        if match:
            addr = int(match.group(1), 16)
            width = 2 * len(match.group(2).split())
            mnemonic = match.group(3).split(";")[0].strip()
            rows[addr] = (width, mnemonic)
    return rows


def disassemble(data, address, symbols=None):
    if len(data) % 2:
        raise ValueError("thumb function has an odd byte length")
    symbols = symbols or {}
    rows = objdump(data, address)
    pool_words = set()
    for addr, (_, mnemonic) in rows.items():
        found = LDR_PC.search(mnemonic)
        if found:
            target = (addr + 4 & ~3) + int(found.group(1))
            if target % 4 or not address <= target < address + len(data):
                raise ValueError(f"pool target 0x{target:08x} is out of range")
            pool_words.add(target)
    is_pool = {word + step for word in pool_words for step in range(4)}

    labels = {}
    cursor = address
    while cursor < address + len(data):
        if cursor in pool_words:
            cursor += 4
            continue
        if cursor in is_pool or cursor not in rows:
            raise ValueError(f"cannot decode at 0x{cursor:08x}")
        width, mnemonic = rows[cursor]
        found = TARGET.search(mnemonic)
        if found:
            target = int(found.group(3), 16)
            if address <= target < address + len(data) and target not in is_pool:
                labels.setdefault(target, f".L{len(labels)}")
        cursor += width

    lines = [".syntax unified", "\t.thumb",
             f"\t.global Func_{address:08x}", "\t.thumb_func",
             f"Func_{address:08x}:"]
    cursor = address
    while cursor < address + len(data):
        if cursor in labels:
            lines.append(f"{labels[cursor]}:")
        if cursor in pool_words:
            value = struct.unpack_from("<I", data, cursor - address)[0]
            lines.append(f"\t.4byte {symbols.get(value, f'0x{value:08x}')}")
            cursor += 4
            continue
        width, mnemonic = rows[cursor]
        lines.append("\t" + retarget(mnemonic, labels))
        cursor += width
    return "\n".join(lines) + "\n"


def retarget(mnemonic, labels):
    found = TARGET.search(mnemonic)
    if not found:
        return mnemonic
    target = int(found.group(3), 16)
    if target in labels:
        replacement = labels[target]
    else:
        replacement = f"Func_{target:08x}"
    return mnemonic[:found.start(3) - 2] + replacement
