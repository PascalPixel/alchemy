#include "local.h"
#include "call.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_020029dc(void)
{
    if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1)
    {
        if (!GameFlag_Test(0x305))
            Call3((void (*)())Func_02004da0, 11, 0, 0);
    }
}
