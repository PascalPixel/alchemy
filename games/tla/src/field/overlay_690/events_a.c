#include "local.h"

void Func_0200043c(s32 actor)
{
    s32 msg = (s32)&MsgScene690No01;

    Func_020020e0(msg);
    UiText_OpenMessageAtObject(actor, 0);
    if (Inventory_PromptAndSetObjectMode(Func_020021a8(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_020020e0(msg + 1);
    } else {
        Battle_WaitMode0(20);
        Func_020020e0(msg + 2);
    }
    Func_020020f0(actor, 0);
}

void Func_02000488(void)
{
    Window(5, 70, 9, 7, 5, 43, 1);
}

void Func_020004a8(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;

    if (x == 5 && z == 17) {
        ObjectMotion_Launch(gPartyState.current_owner, 2, 0);
        SetPosCommit(gPartyState.current_owner, 88, 296);
    }
    Func_020021a0(5, 70, 9, 7, 5, 43, 1);
    Func_02002018(4, 25, 5, 17, 1, 1);
    Func_02002030(4, 25, 1, 1, 5, 17);
    GameFlag_SetBit(0xa60);
}

void Func_0200052c(void)
{
    s32 next = *(s16 *)((u8 *)Ram_HeapSlots->event_work + 0x154);

    ObjectMotion_ArmCallback(4, 0xc000, 0);
    Battle_WaitMode0(10);
    Object_GetById(4)->action_flags &= 0xfe;
    CopyCells(96, 14, 72, 14, 2, 1);
    ObjectMotion_OffsetPositionAndResetMotion(4, 0, 4);
    ObjectMotion_CommitCurrentPositionAndActivate(4);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(4), 0);
    Object_SetModeById(4, 13);
    ObjectMotion_OffsetPositionAndResetMotion(4, 0, 16);
    ObjectMotion_CommitCurrentPositionAndActivate(4);
    Object_SetModeById(4, 10);
    Battle_WaitMode0(10);
    Func_020021b0(123);
    Func_02002110(next);
    Func_02002070();
}
