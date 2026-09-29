/* NONMATCHING: resource_370:02000de4; 1016 / 1024 bytes, 431 differing
 * halfwords, 398 wrong instructions, 255 aligned edits (2026-09-27 Sol H3).
 * Sol H3 clamps through the complete typed owner record. Full normalized
 * diff read: duplicated byte clamps and direct +15 accesses are recovered;
 * pointer materialization and shift induction disappear. Frame stays 68/64,
 * the quantity key still spills at sp+0, and the middle pool remains late.
 * Three bounded structural attempts are preserved; stop this axis without
 * exact credit. Sol H2 was 1012 bytes / 261 aligned edits.
 * Sol H2 restores the quantity helper and clamps rank through a byte
 * pointer. Full normalized diff read: the duplicated byte clamp ancestry
 * is recovered, but pointer +15 materialization and a new shift induction
 * remain. Frame 68/64 and quantity-key spill remain; not an exact witness.
 * Sol H1 owns the complete quantity scan in the caller instead of an
 * inline return boundary. Complete normalized diff read: the key still
 * spills at sp+0, the inventory base still hoists, and the frame stays
 * 68 instead of 64. Quantity truncation moves after the sign test and
 * the pool stays late. Rejected; commit preserves this negative witness.
 * TITLE 020002e8 was already exact at base 6b0d228d3: no new function
 * bytes or alignment bytes. Both production ROMs compare byte-identical.
 * Complete owner 02000de4..020011e4, including both literal-pool groups.
 * The save-menu caller passes (unused, password mode, output), then adds
 * a checksum and calls exact Clear_EncodePassword. Import 02009444 calls
 * Item_Get, not a debug routine: canonical ITEM.H pointer return retained.
 * Transfer from exact Inventory_Find / Inventory_GetQuantity: shared
 * OwnerInventoryState, id low nine bits, quantity high five bits. A local
 * complete last-match scan returns the encoded quantity to the bit writer.
 * H1 improves 1004 bytes / 286 edits to 1008 / 258, notably the pack tail,
 * but fails the admission invariant: key still spills, frame still 68/64,
 * inventory base still hoists and the middle pool is still too late. No
 * exact credit. Original baseline remains in parent c8976444b.
 * H2 puts the two-word row advance in the owner-loop increment, matching
 * the reference's 02000fb2 boundary rather than advancing before the level
 * clamp. Its scheduling moves to that boundary, but the complete score,
 * 68-byte frame, key spill and pool displacement remain unchanged. H1 is
 * preserved at 5d817e2d1. Stop after these two supported models; neither
 * licenses a further register/zero/pointer spelling sweep.
 * Integer-domain
 * packing offset restores complete topology. The shared money union gives
 * the reference's one base and +16/+18 accesses. Frame remains 68 / 64 bytes;
 * a word item temporary restores unsigned ldrh without extension. Property
 * key spill, counter allocation and rank reloads remain. */
#include "TYPES.H"
#include "ITEM.H"
#include "OWNER_STATE.H"

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
extern u16 Data_020096d0[6];
extern s32 Data_020096c0[4];
extern u16 Data_020096dc[8];
extern u16 Data_020096ec[23];

struct PasswordMoney {
    u8 unknown_00[16];
    union {
        u32 value;
        struct {
            u16 low;
            u16 high;
        } half;
    } amount;
};

extern struct PasswordMoney gGameState;

/* FAKEMATCH: the inline boundary is retained from the matching experiment,
 * not evidence of an original helper. Inventory_Find and Inventory_GetQuantity
 * prove the shared inventory view
 * and the low-nine-bit id / high-five-bit quantity encoding. The password
 * stores quantity minus one, and its complete scan retains the last match. */
static __inline__ u16 Password_FindItemQuantity(
    struct OwnerInventoryState *state, s32 target)
{
    u16 *code = state->inventory;
    u16 quantity = 0;
    s32 k;

    for (k = 0; k != 15; k++) {
        u32 item = *code++;

        if ((item & 0x1ff) == target)
            quantity = (item & 0xf800) >> 11;
    }
    return quantity;
}

/* FAKEMATCH: inline boundary recovers the duplicated byte clamp ancestry;
 * it is not evidence of an original helper. */
static __inline__ void Password_ClampRank(struct PasswordOwnerState *state)
{
    if (state->rank > 99)
        state->rank = 99;
    if (state->rank == 0)
        state->rank = 1;
}

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
        if (Engine_GameFlagIsSet(Data_020096d0[i]))
            flag_bits |= 1u << i;
    }

    row = rows;
    for (i = 0; i != 4; i++, row += 2) {
        struct PasswordOwnerState *state =
            Engine_OwnerGetState(Data_020096c0[i]);
        struct PasswordStats *stats = &state->stats;
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

        Password_ClampRank(state);
        rank_bits |= state->rank << (i * 7);

        for (j = 0; j != 4; j++)
            value_bits += state->values_f8[j] << (j * 7);

        for (j = 0; j != 15; j++) {
            s32 k;
            u16 item = state->item_codes[j] & 0x1ff;
            for (k = 0; k != 8; k++) {
                if (item == Data_020096dc[k])
                    item_bits |= 1u << k;
            }
        }
    }

    if (mode == 0) {
        p = 39;
        bit = 0;
        for (i = 0; i != 4; i++) {
            struct PasswordOwnerState *state =
                (struct PasswordOwnerState *)Engine_OwnerGetState(Data_020096c0[i]);
            s32 j;
            for (j = 0; j != 15; j++) {
                s32 item;

                Item_Get(state->item_codes[j]);
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
            struct OwnerInventoryState *state =
                (struct OwnerInventoryState *)Engine_OwnerGetState(Data_020096c0[i]);
            s32 j;
            for (j = 0; j != 23; j++) {
                u16 property = Password_FindItemQuantity(state, Data_020096ec[j]);
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
        out[165] = gGameState.amount.half.high;
        out[166] = gGameState.amount.value >> 8;
        out[167] = gGameState.amount.value;
    }

    if (mode != 2) {
        s32 offset = 8 + (mode != 0);
        u8 *dst = out + offset;

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
