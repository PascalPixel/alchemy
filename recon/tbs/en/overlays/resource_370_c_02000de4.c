/* NONMATCHING: resource_370 password packer, 1024 bytes, 488 lines against 492; the item and
 * quantity passes now allocate as the reference does. What got them there: the item pass counter and the
 * quantity search counter are one function-level variable (n), the found quantity shares
 * the first pass counter (j), the table entry is compared inside the search loop, and the
 * coins byte is the word shifted. The unused narrowing assignment "quantity = j" stands in
 * for whatever left a lone shift above the sign test; a real form is still wanted.
 * Remaining: the first pass keeps its shifted 1 in r12 where the reference uses r10; the
 * item pass orders the bit update and its constants differently; the row pass stores
 * byte 7 once where the reference stores it twice and byte 11 before byte 10. */
#include "TYPES.H"
#include "ITEM.H"
#include "GAME_STATE.H"

struct PasswordStats {
    s16 value_10;
    s16 value_12;
    u8 unknown_14[4];
    u16 value_18;
    u16 value_1a;
    u16 value_1c;
    u8 level_1e;
};

struct PasswordOwner {
    u8 unknown_00[0x0f];
    u8 rank;
    struct PasswordStats stats;
    u8 unknown_20[0xb8];
    u16 items[15];
    u8 unknown_f6[2];
    u32 values[4];
};

s32 Engine_GameFlagIsSet(s32 flag);
struct PasswordOwner *Owner_GetState(s32 owner);
extern const s32 Data_020016c0[4];
extern const u16 Data_020016d0[6];
extern const u16 Data_020016dc[8];
extern const u16 Data_020016ec[23];

s32 Func_02000de4(s32 unused, s32 mode, u8 *out)
{
    s32 length;
    u32 rank_bits;
    u32 value_bits;
    u8 item_bits;
    u8 flag_bits;
    u32 rows[8];
    s32 i;
    s32 p;
    s32 bit;
    s32 j;
    s32 k;
    s32 n;

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

    rank_bits = 0;
    value_bits = 0;
    item_bits = 0;
    flag_bits = 0;
    for (i = 0; i != 8; i++) {
        rows[i] = 0;
    }

    for (i = 0; i != 6; i++) {
        if (Engine_GameFlagIsSet(Data_020016d0[i])) {
            flag_bits |= 1 << i;
        }
    }

    for (i = 0; i != 4; i++) {
        struct PasswordOwner *owner = Owner_GetState(Data_020016c0[i]);
        struct PasswordStats *stats = &owner->stats;

        if (stats->value_10 > 1999) {
            stats->value_10 = 1999;
        }
        if (stats->value_10 < 0) {
            stats->value_10 = 0;
        }
        if (stats->value_12 > 1999) {
            stats->value_12 = 1999;
        }
        if (stats->value_12 < 0) {
            stats->value_12 = 0;
        }
        if (stats->value_18 > 999) {
            stats->value_18 = 999;
        }
        if (stats->value_1a > 999) {
            stats->value_1a = 999;
        }
        if (stats->value_1c > 999) {
            stats->value_1c = 999;
        }
        if (stats->level_1e > 99) {
            stats->level_1e = 99;
        }
        rows[i * 2] = stats->value_10 << 21 | stats->value_12 << 10 | stats->value_18;
        rows[i * 2 + 1] = stats->value_1a << 22 | stats->value_1c << 12 | stats->level_1e << 4;
        if (owner->rank > 99) {
            owner->rank = 99;
        }
        if (owner->rank == 0) {
            owner->rank = 1;
        }
        rank_bits |= owner->rank << (i * 7);
        for (j = 0; j != 4; j++) {
            value_bits += owner->values[j] << (j * 7);
        }
        for (j = 0; j != 15; j++) {
            s32 k;

            for (k = 0; k != 8; k++) {
                if ((owner->items[j] & 0x1ff) == Data_020016dc[k]) {
                    item_bits |= 1 << k;
                }
            }
        }
    }

    if (mode == 0) {
        p = 39;
        bit = 0;
        for (i = 0; i != 4; i++) {
            struct PasswordOwner *owner = Owner_GetState(Data_020016c0[i]);

            for (n = 0; n != 15; n++) {
                s32 item;

                Item_Get(owner->items[n]);
                item = owner->items[n];
                item &= 0x1ff;
                out[p] += item >> (bit + 1);
                out[p + 1] += item << (7 - bit);
                bit++;
                p++;
                if (bit == 7) {
                    bit = 0;
                    p++;
                }
            }
        }

        p = 107;
        bit = -1;
        for (i = 0; i != 4; i++) {
            struct PasswordOwner *owner = Owner_GetState(Data_020016c0[i]);
            s32 m;

            for (m = 0; m != 23; m++) {
                u16 quantity;
                s32 k;

                j = 0;
                for (n = 0; n != 15; n++) {
                    if ((owner->items[n] & 0x1ff) == Data_020016ec[m]) {
                        j = (owner->items[n] & 0xf800) >> 11;
                    }
                }
                quantity = j;
                if (bit < 0) {
                    out[p] += (u16)j >> -bit;
                    p++;
                    bit += 8;
                }
                out[p] += (u16)j << bit;
                bit -= 5;
                if (bit == -5) {
                    p++;
                    bit = 3;
                }
            }
        }
        out[165] = (u32)gGameState.coins >> 16;
        out[166] = (u32)gGameState.coins >> 8;
        out[167] = gGameState.coins;
    }

    if (mode != 2) {
        s32 offset = 8 + (mode != 0);
        u8 *dst = out + offset;

        for (i = 0; i != 2; i++) {
            dst[0] = rows[i * 4] >> 24;
            dst[1] = rows[i * 4] >> 16;
            dst[2] = rows[i * 4] >> 8;
            dst[3] = rows[i * 4];
            dst[4] = rows[i * 4 + 1] >> 24;
            dst[5] = rows[i * 4 + 1] >> 16;
            dst[6] = rows[i * 4 + 1] >> 8;
            dst[7] = rows[i * 4 + 1];
            dst[7] |= rows[i * 4 + 2] >> 28;
            dst[8] = rows[i * 4 + 2] >> 20;
            dst[9] = rows[i * 4 + 2] >> 12;
            dst[10] = rows[i * 4 + 2] >> 4;
            dst[11] = rows[i * 4 + 2] << 4;
            dst[11] |= rows[i * 4 + 3] >> 28;
            dst[12] = rows[i * 4 + 3] >> 20;
            dst[13] = rows[i * 4 + 3] >> 12;
            dst[14] = rows[i * 4 + 3] >> 4;
            dst += 15;
        }
    }

    out[0] = rank_bits;
    out[1] = rank_bits >> 8;
    out[2] = rank_bits >> 16;
    out[3] = (rank_bits >> 20 & 0xf0) | (value_bits & 0x0f);
    out[4] = value_bits >> 4;
    out[5] = value_bits >> 12;
    out[6] = value_bits >> 20;
    out[7] = flag_bits;
    if (mode != 0) {
        out[8] = item_bits;
    }
    return length;
}
