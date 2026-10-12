#include "local.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_02002128(void)
{
    u8 *work = (u8 *)Ram_HeapSlots->event_work;

    do { } while (0); /* FAKEMATCH: a one-pass block loads the event work before its offset is built (one reordered row). */
    *(work + 0xd38) = 1;
    if (!GameFlag_Test(0x304)) {
        Object_SetPartAttribute(Object_GetById(22), 15);
        Func_02004da0(22, 632 << 16, 312 << 16);
        Object_GetById(22)->scale_x = Object_GetById(22)->scale_y = 0x1000;
    }
}
