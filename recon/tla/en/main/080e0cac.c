/* Near miss, differ 8 (four reordered rows): the twin of The Broken Seal's
 * BattleFx_UpdateDescendingObject, same instructions. The listing schedules
 * every load one step earlier than this source: the angle before the scale
 * add, y before its constant, the pooled zero before the address of active.
 * Empty asm barriers after the angle load and after the y load (through
 * forced temporaries) bring it to differ 2; the last row, inside the final
 * block, no barrier, pointer or temporary reached. */
#include "types.h"

struct DescendingObject {
    u8 unknown_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[0x34];
    u8 active;
};

void Func_080e0cac(struct DescendingObject *obj)
{
    u32 scale;

    obj->scale_y -= 0x280;
    scale = obj->scale_x - 0x280;
    obj->angle += 0x2000;
    obj->y += 1 << 16;
    obj->scale_x = scale;
    if ((s32)scale < 0x3000)
        obj->active = 0;
}
