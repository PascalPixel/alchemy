#include "GLOBAL.H"
#include "TYPES.H"
#include "SCENE.H"
#include "ITEM.H"
#include "GAME_STATE.H"
#include "FIELD_EVENT.H"

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

struct PasswordOwner *Owner_GetState(s32 owner);


s32 Clear_PackPasswordData(s32 unused, s32 mode, u8 *out)
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
        if (Engine_GameFlagIsSet(Clear_PasswordFlags[i])) {
            flag_bits |= 1 << i;
        }
    }

    for (i = 0; i != 4; i++) {
        struct PasswordOwner *owner = Owner_GetState(Clear_PasswordMembers[i]);
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
            u16 item = owner->items[j] & 0x1ff;

            for (k = 0; k != 8; k++) {
                if (item == Clear_PasswordKeyItems[k]) {
                    item_bits |= 1 << k;
                }
            }
        }
    }

    if (mode == 0) {
        p = 39;
        bit = 0;
        for (i = 0; i != 4; i++) {
            struct PasswordOwner *owner = Owner_GetState(Clear_PasswordMembers[i]);

            for (n = 0; n != 15; n++) {
                u16 item;

                Item_Get(owner->items[n]);
                item = owner->items[n] & 0x1ff;
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
            struct PasswordOwner *owner = Owner_GetState(Clear_PasswordMembers[i]);
            s32 m;

            for (m = 0; m != 23; m++) {
                u16 quantity;

                j = 0;
                for (n = 0; n != 15; n++) {
                    if ((owner->items[n] & 0x1ff) == Clear_PasswordStackItems[m]) {
                        j = (owner->items[n] & 0xf800) >> 11;
                    }
                }
                /* FAKEMATCH: the unused 16-bit copy leaves the shift hoisted above the sign test; without it 1095, used in both shifts 585. */
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
        s32 q = 8 + (mode != 0);

        for (i = 0; i != 2; i++) {
            out[q] = rows[i * 4] >> 24;
            out[q + 1] = rows[i * 4] >> 16;
            out[q + 2] = rows[i * 4] >> 8;
            out[q + 3] = rows[i * 4];
            out[q + 4] = rows[i * 4 + 1] >> 24;
            out[q + 5] = rows[i * 4 + 1] >> 16;
            out[q + 6] = rows[i * 4 + 1] >> 8;
            out[q + 7] = rows[i * 4 + 1];
            out[q + 7] |= rows[i * 4 + 2] >> 28;
            out[q + 8] = rows[i * 4 + 2] >> 20;
            out[q + 9] = rows[i * 4 + 2] >> 12;
            out[q + 10] = rows[i * 4 + 2] >> 4;
            out[q + 11] = rows[i * 4 + 2] << 4;
            out[q + 11] |= rows[i * 4 + 3] >> 28;
            out[q + 12] = rows[i * 4 + 3] >> 20;
            out[q + 13] = rows[i * 4 + 3] >> 12;
            out[q + 14] = rows[i * 4 + 3] >> 4;
            q += 15;
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

s32 Clear_EncodePassword(u8 *data, s32 size, u8 *out)
{
    u8 *src;
    s32 key;
    s32 count;
    s32 i;
    s32 j;
    s32 bit;
    s32 index;
    s32 value;
    s32 sum;
    s32 group;
    s32 b;

    key = data[size - 1];
    count = 0;
    for (i = 0; i != size - 1; i++)
        data[i] ^= key;
    bit = 0;
    index = 0;
    sum = 0;
    group = 0;
    do {
        value = 0;
        /* FAKEMATCH: the source address is added as integers, index first, for the reference operand order. */
        for (j = 0, src = (u8 *)(index + (s32)data); j != 6; j++) {
            b = (*src >> (7 - bit)) & 1;
            bit++;
            if (bit == 8) {
                bit = 0;
                src++;
                index++;
            }
            value |= b << (5 - j);
            if (index == size)
                break;
        }
        group++;
        out[count++] = value;
        sum += value;
        if (group == 9) {
            out[count++] = sum & 63;
            sum = 0;
            group = 0;
        }
    } while (index != size);
    for (i = 0; i != count; i++)
        out[i] = (out[i] + i) & 63;
    return count;
}

u16 SceneData_GetBufferCrc16(u32 count, u8 *p)
{
    u32 crc;
    u32 i;
    u32 j;

    i = 0;
    crc = 0xFFFF;
    if (count != 0) {
        do {
            crc ^= *p << 8;
            j = 0;
            do {
                if ((crc & 0x8000) != 0) {
                    crc = (crc << 1) + 0xFFFFEFDF;
                } else {
                    crc = crc << 1;
                }
                j++;
            } while (j != 8);
            i++;
            p++;
        } while (i != count);
    }
    return ~crc;
}
