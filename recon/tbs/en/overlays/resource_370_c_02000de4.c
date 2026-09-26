/* NONMATCHING: resource_370:02000de4; 1020 / 1024 bytes, 469 differing
 * halfwords, 482 wrong instructions, 319 halfword edits. Reconstructed
 * clamp reloads, signed item shifts, property bit order and four-word rows;
 * stack frame is 72 / 64 bytes and outer-counter/packing allocation differs. */
#include "TYPES.H"

struct PasswordStats {
    s16 value_10;
    s16 value_12;
    u8 unknown_14[4];
    u16 value_18;
    u16 value_1a;
    u16 value_1c;
    u8 level_1e;
};

struct PasswordOwnerState {
    u8 unknown_00[0x0f];
    u8 rank;
    struct PasswordStats stats;
    u8 unknown_20[0xb8];
    u16 item_codes[15];
    u8 unknown_f6[2];
    u32 values_f8[4];
};

s32 Engine_GameFlagIsSet(s32 flag);
struct PasswordOwnerState *Engine_OwnerGetState(s32 owner);
void Engine_DebugGetItem(s32 item);

s32 Func_02000de4(s32 unused, s32 mode, u8 *out)
{
    s32 length = 11;
    s32 i;
    s32 p;
    s32 bit;
    u32 rank_bits;
    u32 value_bits;
    u8 item_bits;
    u8 flag_bits;
    u32 rows[8];
    u32 *row;
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
    for (i = 0; i != length; i++)
        out[i] = 0;

    rank_bits = 0;
    value_bits = 0;
    item_bits = 0;
    flag_bits = 0;
    {
        u32 *buf = rows;

        for (i = 0; i != 8; i++)
            *buf++ = 0;
    }

    for (i = 0; i != 6; i++) {
        if (Engine_GameFlagIsSet(flags[i]))
            flag_bits |= 1u << i;
    }

    row = rows;
    for (i = 0; i != 4; i++) {
        struct PasswordOwnerState *state =
            Engine_OwnerGetState(owners[i]);
        struct PasswordStats *stats = &state->stats;
        u32 level;
        s32 j;

        if (stats->value_10 > 0x7cf)
            stats->value_10 = 0x7cf;
        if (stats->value_10 < 0)
            stats->value_10 = 0;
        if (stats->value_12 > 0x7cf)
            stats->value_12 = 0x7cf;
        if (stats->value_12 < 0)
            stats->value_12 = 0;
        if (stats->value_18 > 0x3e7)
            stats->value_18 = 0x3e7;
        if (stats->value_1a > 0x3e7)
            stats->value_1a = 0x3e7;
        if (stats->value_1c > 0x3e7)
            stats->value_1c = 0x3e7;
        if (stats->level_1e > 99)
            stats->level_1e = 99;

        row[0] = ((u32)stats->value_10 << 21) |
                 ((u32)stats->value_12 << 10) | stats->value_18;
        row[1] = ((u32)stats->value_1a << 22) |
                 ((u32)stats->value_1c << 12) | (stats->level_1e << 4);
        row += 2;

        level = state->rank;
        if (level > 99)
            state->rank = level = 99;
        if (level == 0)
            state->rank = 1;
        rank_bits |= state->rank << (i * 7);

        for (j = 0; j != 4; j++)
            value_bits += state->values_f8[j] << (j * 7);

        for (j = 0; j != 15; j++) {
            s32 k;
            u16 item = state->item_codes[j] & 0x1ff;
            for (k = 0; k != 8; k++) {
                if (item == item_keys[k])
                    item_bits |= 1u << k;
            }
        }
    }

    if (mode == 0) {
        p = 39;
        bit = 0;
        for (i = 0; i != 4; i++) {
            struct PasswordOwnerState *state =
                (struct PasswordOwnerState *)Engine_OwnerGetState(owners[i]);
            s32 j;
            for (j = 0; j != 15; j++) {
                s32 item;

                Engine_DebugGetItem(state->item_codes[j]);
                item = state->item_codes[j] & 0x1ff;
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
        for (i = 0; i != 4; i++) {
            struct PasswordOwnerState *state =
                (struct PasswordOwnerState *)Engine_OwnerGetState(owners[i]);
            s32 j;
            for (j = 0; j != 23; j++) {
                u16 property = 0;
                s32 k;
                for (k = 0; k != 15; k++) {
                    u16 item = state->item_codes[k];
                    if ((item & 0x1ff) == property_keys[j])
                        property = (item & 0xf800) >> 11;
                }
                if (bit < 0) {
                    out[p] += property >> -bit;
                    p++;
                    bit += 8;
                }
                out[p] += property << bit;
                bit -= 5;
                if (bit == -5) {
                    p++;
                    bit = 3;
                }
            }
        }
        out[165] = *(u16 *)0x02000252;
        out[166] = *(u32 *)0x02000250 >> 8;
        out[167] = *(u32 *)0x02000250;
    }

    if (mode != 2) {
        u8 *dst = out + 8 + (mode != 0);

        row = rows;
        for (i = 0; i != 2; i++) {
            u32 a;
            u32 b;
            u32 c;
            u32 d;

            a = row[0];
            dst[0] = a >> 24;
            dst[1] = a >> 16;
            dst[2] = a >> 8;
            dst[3] = a;
            b = row[1];
            dst[4] = b >> 24;
            dst[5] = b >> 16;
            dst[6] = b >> 8;
            dst[7] = b;
            c = row[2];
            b |= c >> 28;
            dst[8] = c >> 20;
            dst[9] = c >> 12;
            dst[10] = c >> 4;
            dst[11] = c << 4;
            dst[7] = b;
            d = row[3];
            dst[11] = (c << 4) | (d >> 28);
            dst[12] = d >> 20;
            dst[13] = d >> 12;
            dst[14] = d >> 4;
            dst += 15;
            row += 4;
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
