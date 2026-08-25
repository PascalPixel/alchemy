#include "types.h"
#include "shop.h"

#define ShopCursor_MoveTowardTarget Func_080b0958

struct ShopCursorSprite {
    u8 unknown_00[6];
    u16 x;
    u16 y;
    u8 unknown_0a[0x0a];
    u8 screen_y;
    u8 unknown_1b;
    u16 attributes;
};

void ShopCursor_MoveTowardTarget(struct ShopCursor *cursor)
{
    struct ShopCursorSprite *sprite;
    u32 attributes;
    s32 delta;
    s32 step;

    sprite = (struct ShopCursorSprite *)cursor->anchor;
    if (sprite != NULL) {
        delta = sprite->x - cursor->target_x;
        step = delta / 4;
        if (step < 0)
            step = -step;
        if (delta > 0) {
            if (step != 0)
                sprite->x -= step;
            else
                sprite->x += (u16)-1;
        } else {
            if (delta >= 0)
                goto move_y;
            if (step != 0)
                sprite->x += step;
            else
                sprite->x += 1;
        }
        attributes = sprite->x & 0x1ff;
        attributes |= sprite->attributes & 0xfffffe00u;
        sprite->attributes = attributes;

move_y:
        delta = sprite->y - cursor->target_y;
        step = delta / 4;
        if (step < 0)
            step = -step;
        if (delta > 0) {
            if (step != 0)
                sprite->y -= step;
            else
                sprite->y += (u16)-1;
        } else {
            if (delta >= 0)
                return;
            if (step != 0)
                sprite->y += step;
            else
                sprite->y += 1;
        }
        sprite->screen_y = sprite->y;
    }
}
