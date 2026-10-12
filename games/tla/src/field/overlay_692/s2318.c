#include "local.h"
#include "call.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_02002318(void)
{
    if (*(s8 *)((u8 *)Ram_HeapSlots->event_work + 0xd30) == -1) {
        if (!GameFlag_Test(0x304))
            Call3((void (*)())Func_02004da0, 22, 0, 0);
        if (!GameFlag_Test(0x303))
            Call3((void (*)())Func_02004da0, 21, 0, 0);
    }
}
