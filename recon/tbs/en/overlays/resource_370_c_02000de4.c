/* NONMATCHING: 1024 bytes, candidate 1000, 503 differing halfwords, 447
 * halfword edits (2026-09-25). Scene_RunScene370SequenceA, meant for
 * MENU/CLEAR/F_00DE4.C as a single-overlay unit binding its names at their
 * runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: Fresh typed password serializer covers the three output
 * lengths, six flags, four party records, packed fields, items and
 * properties. One complete owner; allocation and packed-field control flow
 * still differ.
 * WALL: Packed-field control flow and local lifetimes need reconstruction. */
#include "TYPES.H"

struct PasswordOwnerState {
    u8 unknown_00[0x0f];
    u8 rank;
    s16 value_10;
    s16 value_12;
    u8 unknown_14[4];
    u16 value_18;
    u16 value_1a;
    u16 value_1c;
    u8 level_1e;
    u8 unknown_1f[0xb9];
    u16 item_codes[15];
    u8 unknown_f6[2];
    u32 values_f8[4];
};

s32 Engine_GameFlagIsSet(s32 flag);
s32 Engine_OwnerGetState(s32 owner);
void Engine_DebugGetItem(s32 item);

s32 Func_02000de4(s32 unused, s32 mode, u8 *out)
{
    s32 length = 11;
    s32 i;
    s32 p;
    s32 bit;
    u32 rank_bits = 0;
    u32 value_bits = 0;
    u32 flag_bits = 0;
    u32 item_bits = 0;
    u32 rows[8];
    u16 *flags = (u16 *)0x020096d0;
    s32 *owners = (s32 *)0x020096c0;
    u16 *item_keys = (u16 *)0x020096dc;
    u16 *property_keys = (u16 *)0x020096ec;

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
    for (i = 0; i < length; i++)
        out[i] = 0;

    for (i = 0; i < 8; i++)
        rows[i] = 0;

    for (i = 0; i < 6; i++) {
        if (Engine_GameFlagIsSet(flags[i]))
            flag_bits |= 1u << i;
    }

    for (p = 0; p < 4; p++) {
        struct PasswordOwnerState *state =
            (struct PasswordOwnerState *)Engine_OwnerGetState(owners[p]);
        s32 hp = state->value_10;
        s32 max_hp = state->value_12;
        u32 level;
        s32 j;

        if (hp > 0x7cf)
            hp = state->value_10 = 0x7cf;
        if (hp < 0)
            hp = state->value_10 = 0;
        if (max_hp > 0x7cf)
            max_hp = state->value_12 = 0x7cf;
        if (max_hp < 0)
            max_hp = state->value_12 = 0;
        if (state->value_18 > 0x3e7)
            state->value_18 = 0x3e7;
        if (state->value_1a > 0x3e7)
            state->value_1a = 0x3e7;
        if (state->value_1c > 0x3e7)
            state->value_1c = 0x3e7;
        if (state->level_1e > 99)
            state->level_1e = 99;

        level = state->rank;
        if (level > 99)
            state->rank = level = 99;
        if (level == 0)
            state->rank = level = 1;
        rank_bits |= level << (p * 7);

        rows[p * 2] = ((u32)hp << 21) |
                      ((u32)max_hp << 10) |
                      state->value_18;
        rows[p * 2 + 1] = ((u32)state->value_1a << 22) |
                          ((u32)state->value_1c << 12) |
                          (state->level_1e << 4);

        for (j = 0; j < 4; j++)
            value_bits += state->values_f8[j] << (j * 7);

        for (j = 0; j < 15; j++) {
            s32 k;
            u16 item = state->item_codes[j] & 0x1ff;
            for (k = 0; k < 8; k++) {
                if (item == item_keys[k])
                    item_bits |= 1u << k;
            }
        }
    }

    if (mode == 0) {
        p = 39;
        bit = 0;
        for (i = 0; i < 4; i++) {
            struct PasswordOwnerState *state =
                (struct PasswordOwnerState *)Engine_OwnerGetState(owners[i]);
            s32 j;
            for (j = 0; j < 15; j++) {
                u32 item = state->item_codes[j] & 0x1ff;
                Engine_DebugGetItem(item);
                out[p] += item >> (bit + 1);
                out[p + 1] += item << (7 - bit);
                p++;
                bit++;
                if (bit == 7) {
                    p++;
                    bit = 0;
                }
            }
        }

        p = 107;
        bit = -1;
        for (i = 0; i < 4; i++) {
            struct PasswordOwnerState *state =
                (struct PasswordOwnerState *)Engine_OwnerGetState(owners[i]);
            s32 j;
            for (j = 0; j < 23; j++) {
                u32 property = 0;
                s32 k;
                for (k = 0; k < 15; k++) {
                    u16 item = state->item_codes[k];
                    if ((item & 0x1ff) == property_keys[j])
                        property = (item & 0xf800) >> 11;
                }
                if (bit < 0) {
                    out[p] += property >> -bit;
                    p++;
                    bit += 8;
                }
                bit -= 5;
                out[p] += property << bit;
                if (bit == -5) {
                    p++;
                    bit = 3;
                }
            }
        }
        out[165] = *(u8 *)0x02000252;
        out[166] = ((u8 *)0x02000250)[1];
        out[167] = ((u8 *)0x02000250)[0];
    }

    if (mode != 2) {
        u8 *dst = out + (mode == 0 ? 8 : 9);
        for (i = 0; i < 2; i++) {
            u32 a = rows[i * 2];
            u32 b = rows[i * 2 + 1];
            u32 c = rows[i * 2 + 2];
            u32 d = rows[i * 2 + 3];
            dst[0] = a >> 24;
            dst[1] = a >> 16;
            dst[2] = a >> 8;
            dst[3] = a;
            dst[4] = b >> 24;
            dst[5] = b >> 16;
            dst[6] = b >> 8;
            dst[7] = b | (c >> 28);
            dst[8] = c >> 20;
            dst[9] = c >> 12;
            dst[10] = c >> 4;
            dst[11] = (c << 4) | (d >> 28);
            dst[12] = d >> 20;
            dst[13] = d >> 12;
            dst[14] = d >> 4;
            dst += 15;
        }
    }

    out[0] = rank_bits;
    out[1] = rank_bits >> 8;
    out[2] = rank_bits >> 16;
    out[3] = ((rank_bits >> 20) & 0xf0) | (value_bits & 0x0f);
    out[4] = value_bits >> 4;
    out[5] = value_bits >> 12;
    out[6] = value_bits >> 20;
    out[7] = flag_bits;
    if (mode != 0)
        out[8] = item_bits;

    return length;
}
