#include "types.h"
#include "motion_object.h"

/*
 * Core-drive row 0x0808bec0, 1052 bytes. A bottom-up reading of the three
 * unread siblings (Func_0808c2dc/c30c/c3a4,
 * all already exact in src/) plus Func_0808b048/Func_0808b02c (also
 * exact) resolved every open question the first pass over this row
 * left standing, including the sp+28 local-array-size worry (the real
 * bound is Func_08077148()'s roster count, not the 250/252 byte
 * *offsets* the disassembly happens to use elsewhere in the row).
 *
 * Per-frame party-leader/roster tick. Fetches the party leader's
 * struct MotionObject (include/motion_object.h, pre-existing, exact)
 * directly from Data_03001ebc using the exact inlined formula
 * src/0808ba1c.c's Func_0808ba1c itself uses (index*4 + 0x14), then:
 *   1. Caches each roster member's object->field_38 (RosterMember,
 *      below) into a small local array.
 *   2. Looks up the terrain tile under the leader's position -- one of
 *      two lookup shapes selected by Data_03001ebc's own +0x19e mode
 *      flag (the same flag 0x0800c62c and 0x0808b3ec already use),
 *      mirroring 0x0800c62c's own `>>20` fixed-point-to-tile shape for
 *      the default table, or a coarser `>>21`/32x32 grid for mode 3.
 *   3. Double-buffers the tile type and fires Func_0808bde0 (an
 *      exact zone-trigger scan, src/0808bde0.c) when it changes.
 *   4. Runs a damage/effect "tick" gated by a Data_02000240 flag and
 *      the leader object's `target_x != 0x80000000` sentinel check --
 *      NOTE: `target_x` is motion_object.h's own name for this offset
 *      (56), but the value it holds behaves exactly like the
 *      `pushback_38[0] == 0x80000000` "no pushback" sentinel
 *      semantic/main/0800ebec.c and 0x0800c62c both use at the SAME
 *      offset in their own (differently-named) reconstruction of what
 *      is very likely this same underlying object type. Flagging the
 *      naming disagreement rather than silently picking a side, same
 *      as 0800ebec.c's own header comment does for its disagreement
 *      with "Object_0800c150"'s field guesses -- used here as
 *      `target_x` since that is what the header this row actually
 *      compiles against declares, not because "target" is believed
 *      more correct. Computes a magnitude from the leader's
 *      `speed_limit` via one of Func_0808b048 or Func_0808b02c (both
 *      exact, src/), clamps it through the 0x03000118 IWRAM multiply,
 *      and feeds the result into Func_0808c2dc (confirms that
 *      sibling's own "owner" parameter name is really this scalar,
 *      not an owner id) plus Func_0808c3a4.
 *   5. A second dispatch block on the cached tile type updates two
 *      Data_02000240 countdown-style fields and calls Func_08093874
 *      (exact, semantic/main/08093874.c) up to three times.
 *   6. A second roster walk classifies each member by
 *      object->field_38's sign against 0 and against the loop-1 cache,
 *      feeding Func_0808c30c (the third sibling) and clearing
 *      object->status_131 for members that qualify.
 *
 * Several intermediate offsets on Data_03001ebc and Data_02000240 are
 * touched without an established name (this row's own use is the only
 * evidence so far); left as raw offsets with comments rather than
 * invented names, same convention 0800ebec.c and 0x0800c62c both use.
 * Not yet checked byte-exact.
 */

/* RosterMember: Func_08077008(id)'s return type. field_34/status_131
 * confirmed exact via the three siblings (src/0808c2dc.c,
 * src/0808c30c.c, src/0808c3a4.c). field_38 is new to this
 * row: read unsigned in the cache pass and signed in the classify
 * pass (an `ldrh` feeding a later `ldrsh` read-back, not a real
 * signedness conflict), compared against both 0 and a per-member
 * cached copy of itself. Uncertainty: its role -- a countdown, a
 * pending-damage amount, a turn-order value -- isn't established by
 * this row alone; named generically rather than guessed at. */
struct RosterMember {
    u8 unknown_00[0x34];
    s16 stat_34; /* confirmed: a core stat, scaled by percentage for
                  * damage/heal amounts (src/0808c30c.c, src/0808c3a4.c) */
    s16 field_38; /* Uncertainty: role not established, see above */
    u8 unknown_3a[0xf5];
    s8 status_131; /* confirmed: a status switch, case 1/2 in
                     * src/0808c3a4.c apply different scaled penalties */
};

extern u8 *Data_03001ebc;
extern u8 Data_02000240[];

/* Called here with r0 still holding Data_03001ebc's own value (nothing
 * reassigns r0 between loading the table pointer and this call) -- the
 * two other siblings (src/0808c30c.c, src/0808c3a4.c) both declare
 * this `(void)` and ignore r0 entirely, so it's treated as unused here
 * too rather than passing the table pointer as if it were meaningful. */
extern s32 Func_08077148(void);
extern u8 *Func_08077008(s32 id);
extern s32 Func_0808d428(s32 kind);
extern void Func_0808bde0(s32 x, s32 y, s32 z);
extern s32 Func_080770c0(s32 query);
extern s32 Func_0808b048(s32 position, s32 speed);
extern s32 Func_0808b02c(u32 index); /* second arg is a hidden r1
    passthrough per its own source comment (src/0808b02c.c) -- called
    here with speed already live in r1 */
extern void Func_0808c2dc(s32 amount);
extern s32 Func_0808c3a4(void);
extern void Func_0808c30c(s32 amount, s32 scaled);
extern void Func_08093874(u32 source_id, s32 flags);
extern void Func_080f9010(s32 sound_id);
extern void Func_08091858(void);

/* Long-distance mode-switching linker veneer, explicitly marked
 * "not expressible in C" in its own source comment (asm/080b50f8.s).
 * Opaque here, same treatment as every other retained-asm callee this
 * dig has hit. */
extern s32 Func_080b50f8(void);

/* IWRAM-relocated fixed-point multiply, opaque -- same helper this reconstruction
 * already treated as opaque for 0x0800c62c and 0x0808b3ec. */
extern s32 Func_03000118(s32 value, s32 multiplier);

void Func_0808bec0(s32 tableSelect, s32 x, s32 y, s32 z)
{
    u8 *table = Data_03001ebc;
    u8 *terrainBase; /* Data_03001e70, read raw since this file never
                       * needs its typed fields */
    u32 leaderHandle;
    struct MotionObject *leader;
    s16 cache[64]; /* real bound is rosterCount below, not a fixed size;
                     * sized generously since the frame's own declared
                     * size isn't available to a semantic draft */
    s32 rosterCount;
    s32 i;
    s32 mode;
    u8 *tile;
    u8 tileType;
    s32 sightingCount = 0;

    terrainBase = *(u8 **)(0x03001e70);

    leaderHandle = *(u32 *)(Data_02000240 + 0x1f4);
    leader = *(struct MotionObject **)(table + leaderHandle * 4 + 0x14);

    if (table == 0) {
        return;
    }

    rosterCount = Func_08077148();
    for (i = 0; i < rosterCount; i++) {
        u8 id = Data_02000240[0x1f8 + i];
        u8 *object = Func_08077008(id);

        cache[i] = *(u16 *)(object + 0x38);
    }

    mode = *(s16 *)(table + 0x19e);
    if (mode == 3) {
        s32 xIndex = (x >= 0) ? x : x + 0x1fffff;
        s32 zIndex = (z >= 0) ? z : z + 0x1fffff;

        xIndex = (xIndex >> 21) & 31;
        zIndex = (zIndex >> 21) & 31;
        tile = *(u8 **)0x02020000 + (xIndex + (zIndex << 5)) * 4;
    } else {
        u8 *base;
        s32 xIndex;
        s32 zIndex;

        if ((u32)tableSelect <= 2) {
            base = *(u8 **)(terrainBase + tableSelect * 48 + 304);
        } else {
            base = *(u8 **)0x02010000;
        }

        xIndex = (x >= 0) ? x : x + 0xfffff;
        zIndex = (z >= 0) ? z : z + 0xfffff;
        xIndex = xIndex >> 20;
        zIndex = zIndex >> 20;
        tile = base + (xIndex + (zIndex << 7)) * 4;
    }

    {
        /* Tile-type change detection, double-buffered against the
           previous two tiles. */
        u8 **previousTileSlot = (u8 **)(table + 0x1b8);
        u8 **priorTileSlot = (u8 **)(table + 0x1bc);
        u8 *previousTile = *previousTileSlot;

        tileType = tile[2];
        *priorTileSlot = previousTile;
        *previousTileSlot = tile;

        if (tileType != 0) {
            Func_0808bde0(x, y, z);
        }
        if ((u8)(tileType - 1) <= 238) {
            *(u16 *)(table + 0x16c) = tileType;
        }
        if ((u8)(tileType - 252) <= 2) {
            *(u16 *)(table + 0x16e) = tileType;
        }
    }

    if (Data_02000240[0x1f2] == 0 && leader != 0 &&
        leader->target_x != (s32)0x80000000) {
        s32 speed = leader->speed_limit;
        s32 magnitude;
        s32 roundFlag;

        if (Func_080770c0(0x167) != 0) {
            speed <<= 1;
        }

        if (mode == 3) {
            magnitude = Func_0808b048((s32)&leader->x, speed);
            roundFlag = 1;
        } else if ((u8)(tileType - 240) <= 1) {
            roundFlag = table[tileType + 177];
            magnitude = Func_0808b02c(tileType - 239); /* speed live in r1 */
        } else {
            roundFlag = table[0x1a0];
            magnitude = Func_0808b02c(0); /* speed live in r1 */
        }
        *(u16 *)(table + 0x17c) = (u16)magnitude;

        {
            s32 scaled = Func_03000118(*(s32 *)(table + 0x1b0), leader->speed_limit);
            s32 *accumulator = (s32 *)(table + 0x1b4);
            s32 carry;

            if (roundFlag == 0) {
                scaled = (scaled + ((u32)scaled >> 31)) >> 1;
            }
            scaled = *accumulator + scaled;

            if (scaled > 0xffff) {
                s32 rounded = (scaled >= 0) ? scaled : scaled + 0xffff;

                *accumulator = scaled & 0xffff;
                carry = rounded >> 16;

                Func_0808c2dc(carry);
                if (Func_080b50f8() != 0) {
                    Func_080f9010(139);
                }
                Func_08091858();
                sightingCount = Func_0808c3a4();
            } else {
                *accumulator = scaled;
            }
        }
    }

    /* Second dispatch: reacts to a specific tile-type coincidence
       (current tile 250, tile-before-previous also 250) by nudging a
       Data_02000240 accumulator from the leader's own speed; otherwise
       folds a different Data_02000240 field toward it directly. */
    if (*(s16 *)(Data_02000240 + 0x22e) == 0 && tileType == 250) {
        u8 *priorTile = *(u8 **)(table + 0x1bc);

        if (priorTile[2] == 250) {
            s32 speed = leader->speed_limit;
            s16 *accumulator = (s16 *)(Data_02000240 + 0x232);

            speed = (speed >= 0) ? speed : speed + 0xffff;
            *accumulator = (s16)(*accumulator + (speed >> 16));
        } else {
            s16 value = *(s16 *)(Data_02000240 + 0x22c);
            s32 rounded = (value + ((u32)value >> 31)) >> 1;

            *(s16 *)(Data_02000240 + 0x232) = (s16)rounded;
        }
    }

    if (*(u32 *)(Data_02000240 + 0x244) != 0 &&
        *(s16 *)(Data_02000240 + 0x23e) != 2) {
        s32 *countdown = (s32 *)(Data_02000240 + 0x244);

        *countdown -= leader->speed_limit;
        if (*countdown <= 0) {
            s16 *flag = (s16 *)(table + 0x17e);

            *countdown = 1;
            if (*flag == 0) {
                *flag = 0x2096;
            }
        }
    }

    if (*(s16 *)(Data_02000240 + 0x22e) == 1) {
        s16 *counter232 = (s16 *)(Data_02000240 + 0x232);
        s16 value22c = *(s16 *)(Data_02000240 + 0x22c);
        s32 rounded22c = (value22c + ((u32)value22c >> 31)) >> 1;

        *counter232 = (s16)(*counter232 + 1);

        if (*counter232 == rounded22c) {
            Func_08093874(leaderHandle, 0x101);
        }
        if (*counter232 == value22c) {
            Func_08093874(leaderHandle, 0x100);
        }
    }

    if (*(s16 *)(Data_02000240 + 0x232) >= *(s16 *)(Data_02000240 + 0x22c)) {
        s16 value230 = *(s16 *)(Data_02000240 + 0x230);

        *(s16 *)(Data_02000240 + 0x232) = 0;
        Func_0808c30c(-(value230 & 0xff), value230 & 0x100);
        sightingCount++;
    }

    if (sightingCount == 0) {
        return;
    }

    *(u16 *)(table + 0x184) = 0;
    *(u16 *)(table + 0x186) = 0;
    leader->velocity_y = 0x40000;
    Func_08093874(leaderHandle, 0x102);

    for (i = 0; i < rosterCount; i++) {
        u8 id = Data_02000240[0x1f8 + i];
        u8 *object = Func_08077008(id);
        s16 field38 = *(s16 *)(object + 0x38);

        if (field38 > 0) {
            *(u16 *)(table + 0x186) += 1;
        } else if (cache[i] != 0) {
            u16 *slotSentinel = (u16 *)(table + 0x182);
            u16 *counter1 = (u16 *)(table + 0x184);
            u16 index = *counter1;

            *counter1 = (u16)(index + 1);
            *(u16 *)(table + 0x188 + index * 2) = id;
            *slotSentinel = 0xffff;
            *(s8 *)(object + 0x131) = 0;
        }
    }
}
