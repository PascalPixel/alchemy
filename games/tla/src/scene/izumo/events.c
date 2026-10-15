#include "izumo.h"

const u8 *Func_020001ac(void)
{
    if (GameFlag_Test(2343))
        return Data_02001644;
    return Data_02001494;
}

void Func_020001cc(void)
{
    Call7((void (*)())Func_02000df8, 53, 5, 8, 9, 23, 30, 0);
}

void Func_020001ec(void)
{
    struct FieldActor *object = Object_GetById(gPartyState.currentOwner);

    Call7((void (*)())Func_02000df8, 53, 5, 8, 9, 23, 30, 0);
    if (!GameFlag_Test(3999)) {
        Call3((void (*)())Func_02000d40, 64, 0x1c80000, 0x2480000);
        if (object->x >> 20 == 28 && object->z >> 20 == 36) {
            Func_02000d00();
            Func_02000dd0(0);
            Func_02000d90(gPartyState.currentOwner, 258);
            ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner, -16, 0);
            object->velocityY = 0x60000;
            ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
            Func_02000d08();
        }
    }
}

void Func_0200027c(void)
{
    struct FieldActor *object = Object_GetById(gPartyState.currentOwner);

    Call7((void (*)())Func_02000df8, 53, 5, 8, 9, 23, 30, 0);
    if (!GameFlag_Test(4000)) {
        Call3((void (*)())Func_02000d40, 65, 0x1d80000, 0x2280000);
        if (object->x >> 20 == 29 && object->z >> 20 == 34) {
            Func_02000d00();
            Func_02000dd0(0);
            Func_02000d90(gPartyState.currentOwner, 258);
            Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, gPartyState.currentOwner, 0, -16);
            object->velocityY = 0x60000;
            ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
            Func_02000d08();
        }
    }
}

void Func_0200030c(void)
{
    Call7((void (*)())Func_02000df8, 44, 5, 8, 5, 7, 32, 0);
}

void Func_0200032c(void)
{
    Call7((void (*)())Func_02000df8, 55, 0, 4, 2, 16, 23, 0);
}

void Func_0200034c(void)
{
    Call7((void (*)())Func_02000df8, 42, 0, 6, 2, 9, 18, 0);
}

void Func_0200036c(void)
{
    struct FieldActor *object = Object_GetById(gPartyState.currentOwner);

    Call7((void (*)())Func_02000df8, 42, 0, 6, 2, 9, 18, 0);
    if (!GameFlag_Test(4001)) {
        Call3((void (*)())Func_02000d40, 66, 0xa80000, 0x1480000);
        if (object->x >> 20 == 10 && object->z >> 20 == 20) {
            Func_02000d00();
            Func_02000dd0(0);
            Func_02000d90(gPartyState.currentOwner, 258);
            ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner, 16, 0);
            object->velocityY = 0x60000;
            ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
            Func_02000d08();
        }
    }
}

void Func_020003f8(void)
{
    Call7((void (*)())Func_02000df8, 49, 0, 5, 4, 24, 16, 0);
}

void Func_02000418(void)
{
    Call7((void (*)())Func_02000df8, 36, 0, 5, 3, 16, 9, 0);
}

void Func_02000438(void)
{
    u8 *work = Ram_HeapSlots->eventWork;

    Func_02000d00();
    Func_02000dd0(0);
    ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
    ObjectMotion_SetActionVariant(gPartyState.currentOwner, 1);
    Object_GetById(gPartyState.currentOwner)->motionFlags = 0;
    Func_02000e08(123);
    Func_02000db0(*(s16 *)(work + 340));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000d08();
}

const u8 *Func_0200049c(void)
{
    if (GameFlag_Test(2343))
        return Data_02001b00;
    return Data_020018b4;
}
