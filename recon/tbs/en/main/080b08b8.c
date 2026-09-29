/* 2026-09-29 alchemy permute: score 1535 to 1400 on the permuter's scorer
   (0 is exact); remaining 18 register-only, 5 reordered, 8 inserted, 2
   deleted. Kept rewrites: 2x introduce a temporary, 1x reorder independent
   statements, 1x add a same-width cast, 1x test truth or compare with
   zero. FAKEMATCH: the permuter's temporaries, register hints and swapped
   operand orders below only steer allocation and scheduling; no programmer
   would write them, so they stay tagged until a natural spelling replaces
   them. */
/* 2026-09-29 alchemy permute: score 2325 to 1535 on the permuter's scorer
   (0 is exact); remaining 14 register-only, 1 operand, 4 reordered, 10
   inserted, 2 deleted. Kept rewrites: 10x change loop form, 9x swap
   commutative operands, 6x add a same-width cast, 5x reorder independent
   statements, 5x introduce a temporary, 4x reorder local declarations, 4x
   drop a same-width cast, 2x remove a temporary, 2x pointer arithmetic or
   indexing, 2x toggle register, 2x test truth or compare with zero, 1x
   split or join a compound assignment. FAKEMATCH: the permuter's
   temporaries, register hints and swapped operand orders below only steer
   allocation and scheduling; no programmer would write them, so they stay
   tagged until a natural spelling replaces them. */
/* Draft, not exact (2026-09-24): 61 differing halfwords at equal size (was 4 bytes short); the reference loads the zero it stores into kind and active from the literal pool.
   Typed sprite fields, widened coordinates, and linked pool masks remove
   the macro but emit 144 bytes (58–64 edits); shortening the zero pulls the
   pool ahead of the second division. Retain the closer 160-byte model.
   FAKEMATCH marks below are empty do-while wraps that only move scheduling
   or register choice; they stay tagged until a real spelling replaces them. */
#include "SHOP.H"

#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 FixedPoint_Ratio(s32, s32);

/*
 * Reconstruction note: struct ShopCursor's six documented fields (anchor,
 * x, y, target_x, target_y, active, kind -- shop.h) line up exactly with
 * this owner's offsets 0/4/6/8/10/12/13, and the anchor writes at +6/+8
 * (ShopCursorAnchor.x/.y) plus the shared "clear low 9 bits, OR in new
 * value" idiom at anchor+22 match the immediately-following sibling
 * main:080b0958 (ShopCursor_MoveTowardTarget) byte-for-byte. That sibling
 * is the "ease toward a live target" mover; this owner is the alternate
 * "linear tween over `active` of `kind` frames" mover for the same
 * anchor fields, so it is named as a distinct ShopCursor_* verb rather
 * than reusing MoveTowardTarget's name.
 */
void ShopCursor_Advance(struct ShopCursor *cursor)
{
    s8 kind;
    u8 active;
    struct ShopCursorAnchor *anchor;
    s16 yStart;
    s16 xStart;
    register s32 dx;
    s32 dy;
    u16 x;
    u16 y;
    s8 tmp;
    s8 tmp4;
    s32 tmp2;
    s8 tmp3;

    if (!cursor)
        return;
    (u8)(kind = cursor->kind);
    if (kind == 0)
        return;
    active = cursor->active;
    if (0 != 1) {
        do {
            anchor = cursor[0].anchor;
            if (!0)
                break;
        } while (1 != 0);
    }
    /* FAKEMATCH */
    tmp2 = active + 1;
    xStart = cursor->x;
    active = tmp2;
    (u32)(dx = cursor->target_x - xStart);
    tmp3 = (s8)active;
    cursor->active = tmp3;
    tmp = active;
    while (1) {
        s8 tmp5;
        tmp5 = (s8)active;
        x = cursor->x + FixedPoint_Ratio(dx * tmp5, kind);
        if (!(0 != 0))
            break;
    }
    /* FAKEMATCH */
    anchor->x = x;
    M2C_FIELD(anchor, u16, 0x16) = (0x1ff & x) | (M2C_FIELD(anchor, u16, 0x16) & 0xfe00);
    yStart = cursor->y;
    dy = cursor->target_y - yStart;
    y = cursor->y + FixedPoint_Ratio(dy * tmp, kind);
    anchor->y = y;
    tmp4 = (s8)y;
    M2C_FIELD(anchor, s8, 0x14) = tmp4;
    if ((s8)active == kind) {
        while (1) {
            cursor->kind = (u32)0;
            if (!0)
                break;
        }
        /* FAKEMATCH */
        cursor->active = 0;
    }
}
