/* Near miss, differ 2 of 46: the native reads the map work pointer before it
   copies the id to r5 (ldr r3, [r3, #32]; adds r5, r1, #0); this draft copies
   the id first. The three are tied in the scheduler and the parameter copy
   is first in the stream. */
#include "../../../../../games/tla/src/field/overlay_67c/local.h"

void Func_020011d4(s32 unused, s32 id)
{
    s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->map_work + 0x140);
    s32 i;

    Object_SetPartAttribute(Object_GetById(id), 5);
    Func_02001e78(id);
    Object_SetModeById(id, 3);
    layer[3] -= 0x200000;
    Func_02003d54();
    Func_02003cdc();
    Func_02003d7c();
    Func_02003eac(0);
    Func_02003ef4(0x22c);
    for (i = 31; i >= 0; i--) {
        layer[3] += 0x10000;
        WaitFrames(4);
    }
    Func_02003d84();
    GameFlag_SetBit(0x24b);
}
