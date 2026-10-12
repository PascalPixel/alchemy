#include "ov6a4.h"

s32 Func_02000538(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 428) = 513;
    if ((u16)(gPartyState.entrance - 3) <= 1)
        Object_GetById(gPartyState.current_owner)->priority_flags |= 32;
    Func_02000858(0);
    Func_02002b28(15, 1);
    Func_02002b28(16, 1);
    Object_GetById(gPartyState.current_owner)->priority_flags |= 64;
    Object_GetById(17)->priority_flags |= 128;
    Object_GetById(18)->priority_flags |= 128;
    Object_GetById(19)->priority_flags |= 128;
    Object_GetById(20)->priority_flags |= 128;
    Object_GetById(21)->priority_flags |= 128;
    Object_GetById(22)->priority_flags |= 128;
    Func_02000e80();
    return 0;
}
