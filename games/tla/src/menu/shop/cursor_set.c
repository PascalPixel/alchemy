#include "types.h"
#include "render_input.h"
#include "ram_buffer.h"

struct ShopCursor {
    struct RenderOutput *anchor;
    u16 x;
    u16 y;
    s16 target_x;
    s16 target_y;
    s8 active;
    s8 kind;
};

struct ShopRuntime {
    u8 unknown_000[0x4dc];
    struct ShopCursor cursor;
    u8 unknown_4ec[0x19];
    u8 mode;
};

struct SpriteAttr {
    unsigned y : 8;
    unsigned affine : 2;
    unsigned blend_mode : 2;
    unsigned mosaic : 1;
    unsigned full_color : 1;
    unsigned shape : 2;
    unsigned x : 9;
    unsigned affine_index : 5;
    unsigned size : 2;
};

struct SpriteAttr2 {
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

struct ShopCursorSprite3 {
    struct RenderOutput *next;
    u8 unknown_04[2];
    u16 x;
    u16 y;
    u8 unknown_0a[0x0a];
    struct SpriteAttr2 oam;
};

void Shop_SetCursor(struct ShopCursor *cursor, s32 target_x, s32 target_y, s8 kind);

// Moves the cursor's sprite a quarter of the way to its target on each
// axis, at least one pixel. The Broken Seal's ShopCursor_MoveTowardTarget.
void Func_081089e4(struct ShopCursor *cursor)
{
    struct ShopCursorSprite3 *sprite;
    s32 delta;
    s32 step;
    u32 mask;

    sprite = (struct ShopCursorSprite3 *)cursor->anchor;
    if (sprite != NULL) {
        delta = sprite->x - cursor->target_x;
        step = delta / 4;
        if (step < 0)
            step = -step;
        if (delta > 0) {
            if (step != 0)
                sprite->x = sprite->x - step;
            else
                sprite->x = sprite->x - 1;
        } else if (delta < 0) {
            if (step != 0)
                sprite->x = sprite->x + step;
            else
                sprite->x = sprite->x + 1;
        } else {
            // Neither move: skip the shared sprite update.
            goto y_axis;
        }
        {
            /* FAKEMATCH: the barrier reloads x before the 0x1ff mask is built, and x pinned to r3 leaves the mask r2, as native. Plain, as The Broken Seal writes the update in each branch, the mask starts first (differ 2; three minutes of alchemy permute found nothing better); the barrier alone swaps the two registers (differ 8); devices in each branch keep the two copies apart (differ 82), so both moves share this one. */
            register u32 x asm("r3") = sprite->x;

            asm volatile("");
            mask = 0x1ff;
            mask &= x;
            sprite->oam.x = mask;
        }

    y_axis:
        delta = sprite->y - cursor->target_y;
        step = delta / 4;
        if (step < 0)
            step = -step;
        if (delta > 0) {
            if (step != 0)
                sprite->y = sprite->y - step;
            else
                sprite->y = sprite->y - 1;
            sprite->oam.y = sprite->y;
        } else if (delta < 0) {
            if (step != 0)
                sprite->y = sprite->y + step;
            else
                sprite->y = sprite->y + 1;
            sprite->oam.y = sprite->y;
        }
    }
}

void Shop_SetCursor(
    struct ShopCursor *cursor,
    s32 target_x,
    s32 target_y,
    s8 kind)
{
    struct RenderOutput *anchor = cursor->anchor;

    cursor->x = (u16)anchor->x;
    cursor->y = (u16)anchor->y;
    cursor->target_x = target_x;
    cursor->target_y = target_y;
    cursor->kind = kind;
    cursor->active = 0;
}

// Puts the cursor at x, y at once: both its position and its target, and its
// sprite with them. The Broken Seal's ShopCursor_SetPositionImmediate.
void Func_08108aa8(struct ShopCursor *cursor, s32 x, s32 y)
{
    {
        struct ShopCursorSprite3 *sprite = (struct ShopCursorSprite3 *)cursor->anchor;

        cursor->target_x = x;
        cursor->kind = 1;
        cursor->active = 0;
        cursor->x = x;
        sprite->oam.x = sprite->x = x;
    }
    {
        struct ShopCursorSprite3 *sprite;

        cursor->target_y = y;
        /* FAKEMATCH: the volatile store of y and reload of the anchor keep the reload after the store, as native; plain, the reload is scheduled ahead of it (one reordered pair, differ 2). */
        *(volatile u16 *)&cursor->y = y;
        sprite = *(struct ShopCursorSprite3 *volatile *)&cursor->anchor;
        sprite->oam.y = sprite->y = y;
    }
}
