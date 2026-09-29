/* 2026-09-29 alchemy permute: score 665 to 545 on the permuter's scorer (0
   is exact); remaining 7 register-only, 1 operand, 3 reordered, 1
   inserted, 2 deleted. Kept rewrites: 4x introduce a temporary, 3x reorder
   independent statements, 1x remove a temporary, 1x drop a same-width
   cast, 1x pointer arithmetic or indexing. FAKEMATCH: the permuter's
   temporaries, register hints and swapped operand orders below only steer
   allocation and scheduling; no programmer would write them, so they stay
   tagged until a natural spelling replaces them. */
/* 2026-09-29 alchemy permute: score 975 to 665 on the permuter's scorer (0
   is exact); remaining 8 register-only, 4 operand, 4 reordered, 1
   inserted, 2 deleted. Kept rewrites: 6x introduce a temporary, 5x swap
   commutative operands, 4x pointer arithmetic or indexing, 3x reorder
   independent statements, 3x reorder local declarations, 2x remove a
   temporary, 1x split or join a compound assignment, 1x toggle register.
   FAKEMATCH: the permuter's temporaries, register hints and swapped
   operand orders below only steer allocation and scheduling; no programmer
   would write them, so they stay tagged until a natural spelling replaces
   them. */
#include "TYPES.H"
#include "SHOP.H"

/* NONMATCHING: 74 of 76 bytes, 23 aligned edits. A pointer-containing
 * attribute union restores the anchor reload but changes mask allocation
 * and pool placement (76 bytes, 27 edits). Consuming x in place gives
 * 74 bytes, 24 edits; widening the zero then gives 72 bytes, 27 edits.
 * Retain the closest baseline; aliasing explains the missing reload. */

struct ShopCursorSprite {
    u8 unknown_00[6];
    u16 x;
    u16 y;
    u8 unknown_0a[0x0a];
    u8 screen_y;
    u8 unknown_1b;
    u16 attributes;
};

extern u8 Data_00000000[];

void ShopCursor_SetPositionImmediate(
    struct ShopCursor *cursor,
    s32 x,
    s32 y)
{
    struct ShopCursorSprite *sprite;
    u32 coordinate_mask;
    register s8 active;
    u32 screen_x;
    struct ShopCursorSprite *tmp;
    u32 tmp2;
    struct ShopCursorAnchor *tmp4;
    u32 tmp5;
    struct ShopCursorAnchor *tmp6;
    s32 tmp7;
    s32 tmp3;
    u8 *tmp8;

    tmp4 = cursor->anchor;
    sprite = (struct ShopCursorSprite *)tmp4;
    coordinate_mask = 0xffff;
    tmp8 = Data_00000000;
    tmp3 = (s32)tmp8;
    active = (s8)tmp3;
    cursor->kind = 1;
    sprite->x = x;
    cursor->target_x = x;
    cursor->x = x;
    cursor->active = active;
    tmp7 = ~0x1ff;
    tmp5 = coordinate_mask & x;
    screen_x = tmp5;
    screen_x &= 0x1ff;
    sprite->attributes = screen_x | (sprite->attributes & tmp7);
    tmp6 = cursor->anchor;
    tmp = (struct ShopCursorSprite *)tmp6;
    sprite = tmp;
    tmp2 = y & coordinate_mask;
    cursor->target_y = y;
    cursor->y = y;
    sprite->y = y;
    y = tmp2;
    (*sprite).screen_y = y;
}
