typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 370 party-summary serializer at 0x02000de4.
 *
 * Complete owner: the four-part prologue at 0x02000de4 (`push {r5, r6, r7,
 * lr}`, fp/sl/r9 pushed through r5-r7, r8 pushed through r7, `sub sp, #64`)
 * through the matching unwind and `pop {r1} / bx r1` at 0x020011c0.  The
 * return address is popped into r1, so r0 survives and IS the result
 * (HANDOVER section 0, epilogue rule): the owner returns the byte length it
 * produced, which is also the value it left in the stack slot at [sp, #24].
 *
 * Two literal pools sit inside the span and are skipped by the control-flow
 * walk from the prologue:
 *   0x02001022-0x0200103f, hopped by the `b.n 0x02001040` at 0x02001020 --
 *     one of the "bare forward branch over a pool" cases HANDOVER section 0
 *     warns about, and the reason the pool map here must come from the walk
 *     rather than from a referenced-words listing; and
 *   0x020011d4-0x020011e3, after the epilogue.
 * Everything between them is reached as instructions.
 *
 * Five call sites, matching the inventory's calls=5, all resolved with
 * `bun tools/overlay_call_targets.ts resource_370 --json` (per-site mapping;
 * the tool's textual summary is a histogram, not a mapping):
 *   0x02000e6c -> veneer 0x1454 -> Func_080770c0   (event-flag test)
 *   0x02000e9c -> veneer 0x143c -> Func_08077008   (party-member record)
 *   0x02000fd2 -> veneer 0x143c -> Func_08077008
 *   0x02000fe8 -> veneer 0x1444 -> Func_08077018   (per-item, result unused)
 *   0x02001068 -> veneer 0x143c -> Func_08077008
 * Func_080770c0 is the same import the owner at 0x02000384 tests, which is
 * the independent evidence that it is a flag predicate rather than something
 * that merely returns a value here.
 *
 * Under this overlay's confirmed 0x02008000 link base (witnessed in
 * semantic/overlays/resource_370_c_02000154.c) all four tables are in-image
 * data and sit consecutively:
 *   0x020096c0 (file offset 0x16c0)  4 words   party-member ids
 *   0x020096d0 (0x16d0)              6 u16     event-flag ids
 *   0x020096dc (0x16dc)              8 u16     item ids scanned for presence
 *   0x020096ec (0x16ec)             23 u16     item ids scanned for counts
 *
 * The output is a bit-packed record.  Its layout, per mode:
 *   mode 0  -> 173 bytes: header at 0-7, four 60-bit stat entries at 8-37,
 *              sixty 9-bit item ids from 39, ninety-two 5-bit item counts from
 *              107, and three bytes of identity at 165-167.
 *   mode 1  ->  39 bytes: header at 0-8 (with the item-presence byte), four
 *              60-bit stat entries at 9-38.
 *   mode 2  ->   9 bytes: header only.
 * Any other mode selects length 11 but still writes the 30-byte entry block at
 * offset 9, which would overrun.  That is what the code does; the caller
 * contract is evidently mode in {0, 1, 2}.
 *
 * Other notes and uncertainties.
 *  - The first parameter is never read: r0 is overwritten by `movs r0, #11`
 *    at 0x02000df4 before any use.  It is declared so the mode stays the
 *    second argument.
 *  - Func_08077018's return value is discarded at 0x02000fe8, and the item
 *    halfword is re-loaded from the record afterwards, so the call is made for
 *    its effect on the record (or is a validation the result of which this
 *    owner ignores).  Which of the two is not determined here.
 *  - Stat clamps are applied in place to the live record, not to a copy.
 *  - The 15-byte entry packer stores bytes 7 and 11 twice, first with the
 *    partial value and then with the merged one.  The intermediate stores are
 *    not observable and only the merged values are written here.
 *  - The 9-bit and 5-bit packers accumulate with `+=` into bytes the initial
 *    clear already zeroed, and their bit cursors deliberately carry across
 *    party members.
 */

/* Overlay imports (via the veneer table).  Old-style declarations: overlay
 * imports vary their argument count between call sites. */
s32 Func_080770c0();
u8 *Func_08077008();
s32 Func_08077018();

s32 Func_02000de4(s32 unused, s32 mode, u8 *out)
{
    const u32 *members = (const u32 *)0x020096c0;
    const u16 *flag_ids = (const u16 *)0x020096d0;
    const u16 *present_ids = (const u16 *)0x020096dc;
    const u16 *counted_ids = (const u16 *)0x020096ec;

    u32 packed[8];
    u32 levels;
    u32 djinn;
    u32 present_bits;
    u32 flag_bits;
    s32 length;
    s32 i;
    s32 member;
    s32 slot;
    s32 bit;
    s32 shift;
    u8 *cursor;
    u8 *actor;
    u8 *stats;

    length = 11;
    if (mode == 1) {
        length = 39;
    } else if (mode > 1) {
        if (mode == 2) {
            length = 9;
        }
    } else if (mode == 0) {
        length = 173;
    }

    for (i = 0; i != length; i++) {
        out[i] = 0;
    }

    levels = 0;
    djinn = 0;
    flag_bits = 0;
    present_bits = 0;
    for (i = 0; i != 8; i++) {
        packed[i] = 0;
    }

    /* Six event flags, one bit each, low bit first. */
    for (i = 0; i != 6; i++) {
        if (Func_080770c0(flag_ids[i]) != 0) {
            flag_bits = (flag_bits | (1u << i)) & 0xff;
        }
    }

    for (member = 0; member != 4; member++) {
        u32 word0;
        u32 word1;
        s32 level;

        actor = Func_08077008(members[member]);
        stats = actor + 16;

        /* Clamps, applied to the record itself. */
        if (*(s16 *)(stats + 0) > 1999) {
            *(u16 *)(stats + 0) = 1999;
        }
        if (*(s16 *)(stats + 0) < 0) {
            *(u16 *)(stats + 0) = 0;
        }
        if (*(s16 *)(stats + 2) > 1999) {
            *(u16 *)(stats + 2) = 1999;
        }
        if (*(s16 *)(stats + 2) < 0) {
            *(u16 *)(stats + 2) = 0;
        }
        if (*(u16 *)(stats + 8) > 999) {
            *(u16 *)(stats + 8) = 999;
        }
        if (*(u16 *)(stats + 10) > 999) {
            *(u16 *)(stats + 10) = 999;
        }
        if (*(u16 *)(stats + 12) > 999) {
            *(u16 *)(stats + 12) = 999;
        }
        if (*(u8 *)(stats + 14) > 99) {
            *(u8 *)(stats + 14) = 99;
        }

        /* 11 + 11 + 10 bits, then 10 + 10 + 7 bits above a zero low nibble. */
        word0 = ((u32)*(s16 *)(stats + 0) << 21) |
                ((u32)*(s16 *)(stats + 2) << 10) |
                (u32)*(u16 *)(stats + 8);
        word1 = ((u32)*(u16 *)(stats + 10) << 22) |
                ((u32)*(u16 *)(stats + 12) << 12) |
                ((u32)*(u8 *)(stats + 14) << 4);
        packed[member * 2] = word0;
        packed[member * 2 + 1] = word1;

        /* Level: clamped to 1..99 in the record, then packed 7 bits each. */
        if (actor[15] > 99) {
            actor[15] = 99;
        }
        if (actor[15] == 0) {
            actor[15] = 1;
        }
        level = actor[15];
        levels |= (u32)level << (member * 8 - member);

        /* Four counters summed across the whole party, 7 bits apart. */
        for (i = 0; i != 4; i++) {
            djinn += ((const u32 *)(actor + 248))[i] << (i * 7);
        }

        /* Presence of eight specific items anywhere in the 15 slots. */
        for (slot = 0; slot != 15; slot++) {
            u32 id = *(u16 *)(actor + 216 + slot * 2) & 0x1ff;

            for (i = 0; i != 8; i++) {
                if (id == present_ids[i]) {
                    present_bits = (present_bits | (1u << i)) & 0xff;
                }
            }
        }
    }

    if (mode == 0) {
        /* Every item id in the party, nine bits each, from offset 39. */
        cursor = out + 39;
        bit = 0;
        for (member = 0; member != 4; member++) {
            actor = Func_08077008(members[member]);
            for (slot = 0; slot != 15; slot++) {
                u32 id;

                Func_08077018(*(u16 *)(actor + 216 + slot * 2));
                id = *(u16 *)(actor + 216 + slot * 2) & 0x1ff;

                cursor[0] = (u8)(cursor[0] + (id >> (bit + 1)));
                cursor[1] = (u8)(cursor[1] + (id << (7 - bit)));
                bit++;
                cursor++;
                if (bit == 7) {
                    bit = 0;
                    cursor++;
                }
            }
        }

        /* Held count of 23 specific items per member, five bits each, from
         * offset 107.  The bit cursor starts at -1 and steps by -5. */
        cursor = out + 107;
        shift = -1;
        for (member = 0; member != 4; member++) {
            actor = Func_08077008(members[member]);
            for (i = 0; i != 23; i++) {
                u32 count = 0;

                for (slot = 0; slot != 15; slot++) {
                    u16 entry = *(u16 *)(actor + 216 + slot * 2);

                    if ((entry & 0x1ff) == counted_ids[i]) {
                        count = (entry & 0xf800) >> 11;
                    }
                }

                if (shift < 0) {
                    *cursor = (u8)(*cursor + (count >> -shift));
                    cursor++;
                    shift += 8;
                }
                *cursor = (u8)(*cursor + (count << shift));
                shift -= 5;
                if (shift == -5) {
                    cursor++;
                    shift = 3;
                }
            }
        }

        out[165] = (u8)*(u16 *)((u8 *)0x02000240 + 18);
        out[166] = (u8)(*(u32 *)((u8 *)0x02000240 + 16) >> 8);
        out[167] = (u8)*(u32 *)((u8 *)0x02000240 + 16);
    }

    if (mode != 2) {
        /* Four 60-bit stat entries, two per 15-byte group, big-endian. */
        cursor = out + (mode != 0 ? 9 : 8);
        for (i = 0; i != 2; i++) {
            u32 a = packed[i * 4 + 0];
            u32 b = packed[i * 4 + 1];
            u32 c = packed[i * 4 + 2];
            u32 d = packed[i * 4 + 3];

            cursor[0] = (u8)(a >> 24);
            cursor[1] = (u8)(a >> 16);
            cursor[2] = (u8)(a >> 8);
            cursor[3] = (u8)a;
            cursor[4] = (u8)(b >> 24);
            cursor[5] = (u8)(b >> 16);
            cursor[6] = (u8)(b >> 8);
            cursor[7] = (u8)(b | (c >> 28));
            cursor[8] = (u8)(c >> 20);
            cursor[9] = (u8)(c >> 12);
            cursor[10] = (u8)(c >> 4);
            cursor[11] = (u8)((c << 4) | (d >> 28));
            cursor[12] = (u8)(d >> 20);
            cursor[13] = (u8)(d >> 12);
            cursor[14] = (u8)(d >> 4);

            cursor += 15;
        }
    }

    out[0] = (u8)levels;
    out[1] = (u8)(levels >> 8);
    out[2] = (u8)(levels >> 16);
    out[3] = (u8)(((levels >> 20) & 0xf0) | (djinn & 0x0f));
    out[4] = (u8)(djinn >> 4);
    out[5] = (u8)(djinn >> 12);
    out[6] = (u8)(djinn >> 20);
    out[7] = (u8)flag_bits;
    if (mode != 0) {
        out[8] = (u8)present_bits;
    }

    return length;
}
