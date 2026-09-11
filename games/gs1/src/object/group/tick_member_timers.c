#include "types.h"
#include "scene.h"
#include "abi/object/group/tick_member_timers.h"

extern u8 *gIw;

void ObjectGroup_TickMemberTimers(void)
{
    u8 *base;
    s32 i;
    s32 index;

    base = gIw;
    i = 0;
    do {
        if (base[0x7818 + i] != 0) {
            if ((base[0x7818 + i] = base[0x7818 + i] - 1) == 0) {
                index = i * 2 + 36;
                Obj_SetRange(
                    *(s16 *)(*(u8 **)(base + 0x7828) + index),
                    0, -1, -1, 0);
            }
        }
        i += 1;
    } while (i != 8);
}
