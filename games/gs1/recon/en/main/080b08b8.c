#include "shop.h"

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
    s16 xStart;
    s16 yStart;
    s32 dx;
    s32 dy;
    u16 x;
    u16 y;

    if (cursor == 0)
        return;
    kind = cursor->kind;
    if (kind == 0)
        return;

    anchor = cursor->anchor;
    active = cursor->active;
    active += 1;
    xStart = cursor->x;
    dx = cursor->target_x - xStart;
    cursor->active = active;
    x = cursor->x + FixedPoint_Ratio((s8)active * dx, kind);
    anchor->x = x;
    M2C_FIELD(anchor, u16, 0x16) =
        (M2C_FIELD(anchor, u16, 0x16) & 0xfe00) | (x & 0x1ff);

    yStart = cursor->y;
    dy = cursor->target_y - yStart;
    y = cursor->y + FixedPoint_Ratio((s8)active * dy, kind);
    anchor->y = y;
    M2C_FIELD(anchor, s8, 0x14) = (s8)y;

    if ((s8)active == kind) {
        cursor->kind = 0;
        cursor->active = 0;
    }
}
