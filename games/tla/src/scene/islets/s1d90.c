#include "islets.h"

void Func_02001d90(void)
{
    Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02001cfc, 0x480);
    Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02001cac, 0xc80);
}

void Func_02003fac();
void Func_02003fb4();
void Func_02003fbc();
void Func_02003fc4();
void Func_02003edc();
void ObjectMotion_Launch();
void PartyInventory_GiveItem();

// When object 19 stands in its target area, actor 9 and the leader react and
// the party is given item 0x1c4.
void Func_02001db0(void)
{
    struct ObjectRuntime *object = Object_GetById(19);
    struct ObjectRuntime *leader;
    struct ObjectRuntime *item;
    s32 z;

    if (object->x < 0xe40000 || object->x > 0xec0000)
        return;
    z = object->z;
    /* FAKEMATCH: the one-pass block loads z before 0xa40000 is built; plain, the ARM9TDMI schedule starts the constant first (differ 2), and z's two tests stay apart as the native's are (one range test differs by 71). */
    do {
    } while (0);
    if (z < 0xa40000)
        return;
    if (z > 0xac0000)
        return;
    ((void (*)())Func_02003e64)();
    Call1((void (*)())Func_02003fa4, 0);
    Call2((void (*)())Object_SetModeById, 19, 1);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, gPartyState.current_owner, 216, 184);
    Call2((void (*)())Func_02003fac, gPartyState.current_owner, 9);
    Call1((void (*)())Func_02003f24, (s32)MsgThankYouSo);
    Call3((void (*)())ObjectMotion_Launch, 9, 2, 10);
    Call3((void (*)())ObjectMotion_Launch, 9, 4, 10);
    ObjectMotion_ArmCallback(gPartyState.current_owner, 0, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x8000, 10);
    Call2((void (*)())Func_02003f2c, 9, 0);
    Object_GetById(9)->action_flags &= ~1;
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 9, 222, 186);
    Call1((void (*)())Battle_WaitMode0, 1);
    Object_GetById(9)->action_flags |= 1;
    Object_GetById(9)->action_flags &= ~1;
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 9, 232, 186);
    Call1((void (*)())Battle_WaitMode0, 1);
    Object_GetById(9)->action_flags |= 1;
    Call2((void (*)())Func_02003f3c, gPartyState.current_owner, 0x4000);
    Call2((void (*)())Object_SetModeById, gPartyState.current_owner, 28);
    leader = Object_GetById(gPartyState.current_owner);
    item = Object_GetById(20);
    item->motion_flags = 0;
    Call2((void (*)())Func_02003e3c, (s32)item, 0x1c4);
    Call4((void (*)())Object_SetPositionAndResetMotion, (s32)item, leader->x, leader->y + 0x180000, leader->z);
    Call1((void (*)())Battle_WaitMode0, 20);
    ((void (*)())Func_02003fc4)();
    Value2((s32 (*)())PartyInventory_GiveItem, 0x1c4, 0);
    ((void (*)())Func_02003fbc)();
    Call3((void (*)())Func_02003edc, 20, 0, 0);
    Call2((void (*)())Object_SetModeById, gPartyState.current_owner, 1);
    ((void (*)())Func_02003fb4)();
    Call2((void (*)())Func_02003f3c, 9, 0xb000);
    Call1((void (*)())GameFlag_SetBit, 0xaa2);
    ((void (*)())Func_02003e6c)();
}
