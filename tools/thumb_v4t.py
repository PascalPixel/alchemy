#!/usr/bin/env python3
"""A strict ARMv4T (ARM7TDMI) Thumb disassembler that emits GAS mnemonics.

objdump decodes 32-bit Thumb-2 forms even under -marmv4t, which the ARM7TDMI
cannot execute and arm-none-eabi-as rejects; that makes it useless for the
data-interleaved main code, where the boundary between code and data is exactly
what is in question. This decoder only ever produces the 16-bit encodings (plus
the 32-bit BL pair) the ARM7TDMI actually has, so a byte that is not a real v4t
instruction returns None (the caller emits it as data) instead of a phantom
Thumb-2 mnemonic. Every emitted mnemonic re-assembles to its original bytes;
tools/thumb_v4t_selftest exercises this against the ROM's own claimed code.
"""

LO = [f"r{n}" for n in range(8)]
HI = [f"r{n}" for n in range(8)] + ["r8", "r9", "r10", "r11", "r12", "sp",
                                    "lr", "pc"]
ALU = ["ands", "eors", "lsls", "lsrs", "asrs", "adcs", "sbcs", "rors",
       "tst", "negs", "cmp", "cmn", "orrs", "muls", "bics", "mvns"]
CONDITION = ["eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc",
             "hi", "ls", "ge", "lt", "gt", "le"]


def _reglist(mask, extra=None):
    names = [LO[bit] for bit in range(8) if mask & (1 << bit)]
    if extra:
        names.append(extra)
    return "{" + ", ".join(names) + "}"


def disassemble(halfword, following, address):
    """Decode one ARMv4T Thumb instruction.

    Returns (text, width, target) where target is the absolute branch/call
    destination for b/bcc/bl (else None), or None if the halfword is not a
    valid ARMv4T instruction start (the caller emits those bytes as data).
    """
    h = halfword
    top = h >> 12

    if top in (0, 1):
        op = (h >> 11) & 3
        if op != 3:                                   # LSL/LSR/ASR immediate
            rd, rs = h & 7, (h >> 3) & 7
            imm = (h >> 6) & 0x1f
            return f"{['lsls', 'lsrs', 'asrs'][op]} {LO[rd]}, {LO[rs]}, #{imm}", 2, None
        rd, rs = h & 7, (h >> 3) & 7                   # ADD/SUB register/imm3
        rn = (h >> 6) & 7
        sub = (h >> 9) & 1
        if (h >> 10) & 1:                             # immediate 3
            return f"{'subs' if sub else 'adds'} {LO[rd]}, {LO[rs]}, #{rn}", 2, None
        return f"{'subs' if sub else 'adds'} {LO[rd]}, {LO[rs]}, {LO[rn]}", 2, None

    if top in (2, 3):                                  # mov/cmp/add/sub imm8
        op = (h >> 11) & 3
        rd, imm = (h >> 8) & 7, h & 0xff
        return f"{['movs', 'cmp', 'adds', 'subs'][op]} {LO[rd]}, #{imm}", 2, None

    if top == 4:
        if h < 0x4400:                                 # ALU operations
            op = (h >> 6) & 0xf
            rd, rs = h & 7, (h >> 3) & 7
            if op == 9:                                # NEG is RSBS Rd,Rs,#0
                return f"negs {LO[rd]}, {LO[rs]}", 2, None
            return f"{ALU[op]} {LO[rd]}, {LO[rs]}", 2, None
        if h < 0x4800:                                 # hi register ops / BX
            op = (h >> 8) & 3
            rd = (h & 7) | ((h >> 4) & 8)
            rs = (h >> 3) & 0xf
            if op == 3:
                if h & 0x80:                           # BLX is ARMv5, not v4t
                    return None
                return f"bx {HI[rs]}", 2, None
            return f"{['add', 'cmp', 'mov'][op]} {HI[rd]}, {HI[rs]}", 2, None
        rd, imm = (h >> 8) & 7, h & 0xff               # PC-relative load
        target = ((address + 4) & ~3) + imm * 4
        return f"ldr {LO[rd]}, [pc, #{imm * 4}]", 2, None

    if top == 5:                                       # load/store reg offset
        rd, rb, ro = h & 7, (h >> 3) & 7, (h >> 6) & 7
        kind = (h >> 9) & 7
        name = ["str", "strh", "strb", "ldrsb",
                "ldr", "ldrh", "ldrb", "ldrsh"][kind]
        return f"{name} {LO[rd]}, [{LO[rb]}, {LO[ro]}]", 2, None

    if top in (6, 7):                                  # load/store word/byte imm
        rd, rb = h & 7, (h >> 3) & 7
        imm = (h >> 6) & 0x1f
        byte = (h >> 12) & 1
        load = (h >> 11) & 1
        scale = 1 if byte else 4
        name = ("ldrb" if byte else "ldr") if load else ("strb" if byte else "str")
        return f"{name} {LO[rd]}, [{LO[rb]}, #{imm * scale}]", 2, None

    if top == 8:                                       # load/store halfword imm
        rd, rb = h & 7, (h >> 3) & 7
        imm = (h >> 6) & 0x1f
        load = (h >> 11) & 1
        return f"{'ldrh' if load else 'strh'} {LO[rd]}, [{LO[rb]}, #{imm * 2}]", 2, None

    if top == 9:                                       # SP-relative load/store
        rd, imm = (h >> 8) & 7, h & 0xff
        load = (h >> 11) & 1
        return f"{'ldr' if load else 'str'} {LO[rd]}, [sp, #{imm * 4}]", 2, None

    if top == 0xa:                                     # load address (adr/add sp)
        rd, imm = (h >> 8) & 7, h & 0xff
        if (h >> 11) & 1:
            return f"add {LO[rd]}, sp, #{imm * 4}", 2, None
        return f"add {LO[rd]}, pc, #{imm * 4}", 2, None

    if top == 0xb:
        if h < 0xb100:                                 # add/sub SP immediate
            imm = h & 0x7f
            return f"{'sub' if (h >> 7) & 1 else 'add'} sp, #{imm * 4}", 2, None
        if 0xb400 <= h < 0xb600:                       # PUSH
            if not (h & 0xff) and not (h & 0x100):     # empty list: invalid
                return None
            extra = "lr" if h & 0x100 else None
            return f"push {_reglist(h & 0xff, extra)}", 2, None
        if 0xbc00 <= h < 0xbe00:                       # POP
            if not (h & 0xff) and not (h & 0x100):     # empty list: invalid
                return None
            extra = "pc" if h & 0x100 else None
            return f"pop {_reglist(h & 0xff, extra)}", 2, None
        return None                                    # bkpt/cbz/nop: not v4t

    if top == 0xc:                                     # multiple load/store
        rb, mask = (h >> 8) & 7, h & 0xff
        if not mask:                                   # empty list: invalid
            return None
        load = (h >> 11) & 1
        return f"{'ldmia' if load else 'stmia'} {LO[rb]}!, {_reglist(mask)}", 2, None

    if top == 0xd:                                     # conditional branch / SWI
        cond = (h >> 8) & 0xf
        if cond == 0xf:
            return f"swi #{h & 0xff}", 2, None
        if cond == 0xe:                                # undefined
            return None
        offset = (h & 0xff)
        offset = offset - 0x100 if offset & 0x80 else offset
        target = address + 4 + offset * 2
        return f"b{CONDITION[cond]} 0x{target:08x}", 2, target

    if top == 0xe:
        if h < 0xe800:                                 # unconditional branch
            offset = h & 0x7ff
            offset = offset - 0x800 if offset & 0x400 else offset
            target = address + 4 + offset * 2
            return f"b 0x{target:08x}", 2, target
        return None                                    # BLX suffix: ARMv5

    # top == 0xf: BL, a 32-bit pair of 0xF0xx (high) then 0xF8xx (low).
    if h < 0xf800:
        if following is None or not 0xf800 <= following <= 0xffff:
            return None
        high = h & 0x7ff
        high = high - 0x800 if high & 0x400 else high
        offset = (high << 12) | ((following & 0x7ff) << 1)
        target = address + 4 + offset
        return f"bl 0x{target:08x}", 4, target
    return None                                        # orphan BL low half
