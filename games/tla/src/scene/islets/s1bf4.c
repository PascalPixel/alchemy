#include "islets.h"

void Func_02001bf4(void)
{
    Func_02003e64();
    Func_02003fa4(0);
    ObjectMotion_SetAngleToward(9, gPartyState.currentOwner, 0);
    Func_02003f24((s32)MsgMyPopS);
    Func_02003f2c(9, 0);
    Call2((void (*)())Func_02003f3c, 9, 0xb000);
    Func_02003e6c();
}

void Func_02001c38(void)
{
    struct IsletsEventWork *work = Ram_HeapSlots->eventWork;

    work->field15a = 1;
}

void Func_02001c4c(void)
{
    s32 y = Object_GetById(gPartyState.currentOwner)->y;

    asm volatile("" : "+l"(y)); /* FAKEMATCH: the listing loads y before it builds 0x280000 */
    if (y < 0x280000) {
        Object_GetById(11)->collisionFlags |= 8;
        Object_GetById(12)->collisionFlags |= 8;
    } else {
        Object_GetById(11)->collisionFlags &= ~8;
        Object_GetById(12)->collisionFlags &= ~8;
    }
}
