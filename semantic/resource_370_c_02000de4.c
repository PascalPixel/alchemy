#include "types.h"

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
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_370 --json` (per-site mapping;
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
    const u16 *flag_ids = (const u16 *)0x020096d0;
    const u16 *present_ids = (const u16 *)0x020096dc;
    const u16 *counted_ids = (const u16 *)0x020096ec;

    u32 packed[8];
    s32 length;
    u32 levels;
    u32 djinn;
    u8 present_bits;
    u8 flag_bits;
    u32 i;
    s32 j;
    s32 k;

    length = 11;
    switch (mode) {
    case 0:
        length = 173;
        break;
    case 1:
        length = 39;
        break;
    case 2:
        length = 9;
        break;
    }

    for (i = 0; i != length; i++) {
        out[i] = 0;
    }

    djinn = 0;
    levels = 0;
    flag_bits = 0;
    present_bits = 0;
    for (i = 0; i != 8; i++) {
        packed[i] = 0;
    }

    /* Six event flags, one bit each, low bit first. */
    for (i = 0; i != 6; i++) {
        if (Func_080770c0(flag_ids[i]) != 0) {
            flag_bits |= 1u << i;
        }
    }

    for (i = 0; i != 4; i++) {
        u32 word0;
        u32 word1;
        s32 level;
        u8 *actor;
        u8 *stats;

        actor = Func_08077008(((const u32 *)0x020096c0)[i]);
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
        packed[i * 2] = word0;
        packed[i * 2 + 1] = word1;

        /* Level: clamped to 1..99 in the record, then packed 7 bits each. */
        if (actor[15] > 99) {
            actor[15] = 99;
        }
        if (actor[15] == 0) {
            actor[15] = 1;
        }
        level = actor[15];
        levels |= (u32)level << (i * 8 - i);

        /* Four counters summed across the whole party, 7 bits apart. */
        for (j = 0; j != 4; j++) {
            djinn += ((const u32 *)(actor + 248))[j] << (j * 7);
        }

        /* Presence of eight specific items anywhere in the 15 slots. */
        for (j = 0; j != 15; j++) {
            u32 id = *(u16 *)(actor + 216 + j * 2) & 0x1ff;

            for (k = 0; k != 8; k++) {
                if (id == present_ids[k]) {
                    present_bits |= 1u << k;
                }
            }
        }
    }

    if (mode == 0) {
        s32 out_pos;
        s32 bit;
        s32 party_index;
        s32 item_index;
        s32 counted_index;
        u32 item_pos;

        /* Every item id in the party, nine bits each, from offset 39. */
        out_pos = 39;
        bit = 0;
        for (party_index = 0; party_index != 4; party_index++) {
            u8 *actor;

            actor = Func_08077008(
                ((const volatile u32 *)0x020096c0)[party_index]);
            item_pos = 216;
            item_index = 0;
            do {
                u32 id;

                Func_08077018(*(u16 *)(actor + item_pos));
                id = *(u16 *)(actor + item_pos) % 0x200;

                out[out_pos] = (u8)(out[out_pos] + (id >> (bit + 1)));
                out[out_pos + 1] =
                    (u8)(out[out_pos + 1] + (id << (7 - bit)));
                bit++;
                out_pos++;
                if (bit == 7) {
                    bit = 0;
                    out_pos++;
                }
                item_index++;
                item_pos += 2;
            } while (item_index != 15);
        }

        /* Held count of 23 specific items per member, five bits each, from
         * offset 107.  The bit cursor starts at -1 and steps by -5. */
        out_pos = 107;
        bit = -1;
        for (party_index = 0; party_index != 4; party_index++) {
            u8 *actor;

            actor = Func_08077008(
                ((const volatile u32 *)0x020096c0)[party_index]);
            for (counted_index = 0; counted_index != 23; counted_index++) {
                u32 count = 0;
                u16 target_id = counted_ids[counted_index];

                for (item_index = 0; item_index != 15; item_index++) {
                    u16 entry = *(u16 *)(actor + 216 + item_index * 2);
                    u32 id = entry % 0x200;

                    if (id == target_id) {
                        count = (entry & 0xf800) >> 11;
                    }
                }

                if (bit < 0) {
                    out[out_pos] =
                        (u8)(out[out_pos] + (count >> -bit));
                    out_pos++;
                    bit += 8;
                }
                out[out_pos] = (u8)(out[out_pos] + (count << bit));
                bit -= 5;
                if (bit == -5) {
                    out_pos++;
                    bit = 3;
                }
            }
        }

        out[165] = (u8)*(u16 *)((u8 *)0x02000240 + 18);
        out[166] = (u8)(*(u32 *)((u8 *)0x02000240 + 16) >> 8);
        out[167] = (u8)*(u32 *)((u8 *)0x02000240 + 16);
    }

    if (mode != 2) {
        /* Four 60-bit stat entries, two per 15-byte group, big-endian. */
        u8 *cursor = out + (mode != 0 ? 9 : 8);
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
    out[4] = (u8)(djinn >> 4);
    out[2] = (u8)(levels >> 16);
    out[3] = (u8)(((levels >> 20) & 0xf0) | (djinn & 0x0f));
    out[7] = (u8)flag_bits;
    out[6] = (u8)(djinn >> 20);
    out[5] = (u8)(djinn >> 12);
    if (mode != 0) {
        out[8] = (u8)present_bits;
    }

    return length;
}
