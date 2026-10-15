#include "caverns.h"

void Func_02000d68(void)
{
    s32 value = HALF_AT(Ram_HeapSlots->eventWork, 340);

    if (value == 2)
        Call3((void (*)())Func_0200110c, 8, 0x900000, 0xe00000);
    else
        Call3((void (*)())Func_0200110c, 8, 0xe00000, 0xe00000);
    ObjectMotion_ArmCallback(4, 0xc000, 0);
    Battle_WaitMode0(10);
    BYTE_AT(Object_GetById(4), 90) &= ~1;
    Call6((void (*)())Func_02001094, 96, 14, 72, 14, 2, 1);
    ObjectMotion_OffsetPositionAndResetMotion(4, 0, 4);
    ObjectMotion_CommitCurrentPositionAndActivate(4);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(4), 0);
    ObjectMotion_OffsetPositionAndResetMotion(4, 0, 16);
    ObjectMotion_CommitCurrentPositionAndActivate(4);
    Object_SetModeById(4, 13);
    Func_02001164(123);
    Battle_WaitMode0(14);
    Func_02001134(value);
}
