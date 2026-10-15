#include "types.h"
#include "inventory.h"
#include "call.h"
#include "ov677.h"

s32 Func_02000a90(void);
void Func_02000a60(s32 actor, s32 angle);
void Func_02000a80(s32 actor);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

s32 Func_020004e0(void)
{
    s32 item;

    for (item = 429; item <= 437; item++) {
        if (PartyInventory_FindOwner(item) != -1)
            return 1;
    }
    for (item = 417; item <= 427; item++) {
        if (PartyInventory_FindOwner(item) != -1)
            return 1;
    }
    return 0;
}

// Actor 14's talk. Flag 0x10f follows Func_020004e0's item check (it stays
// set while flag 0x301 or Func_02000a90 holds). Once 0x10f is set and
// neither holds, a yes moves actor 14 and the leader, and if Func_02000a90
// then holds, places actor 16 and sets flag 0x301.
void Func_02000524(void)
{
    s32 message;

    if (Func_020004e0())
        Ov677_FlagSet(0x10f);
    else if (!Ov677_FlagCheck(0x301) && !Func_02000a90())
        GameFlag_ClearBit(0x10f);

    if (!Ov677_FlagCheck(0x10f)) {
        message = (s32)MsgAhSoYou;
        Func_02000a18(message);
        Ov677_MessageOpen(14, 0);
        if (Inventory_PromptAndSetObjectMode(Func_02000a58(), 0) == 0) {
            Ov677_Wait(10);
            Func_02000a18(message + 1);
        } else {
            Ov677_Wait(20);
            Func_02000a18(message + 2);
        }
        Ov677_MessagePut(14, 0);
    } else if (Ov677_FlagCheck(0x301)) {
        Func_02000a18((s32)MsgICanTWait);
        Ov677_MessagePut(14, 0);
    } else if (Func_02000a90()) {
        Func_02000a18((s32)MsgMyWifeHas);
        Ov677_MessagePut(14, 0);
    } else {
        message = (s32)MsgYouWantMe;
        Func_02000a18(message);
        Ov677_MessageOpen(14, 0);
        if (Inventory_PromptAndSetObjectMode(Func_02000a58(), 0) == 0) {
            Func_02000a18(message + 1);
            Ov677_MessagePut(14, 0);
            Call2((void (*)())Func_02000a60, 14, 0xc000);
            ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 1);
            Call3((void (*)())ObjectMotion_ResetAndSetPosition, gPartyState.current_owner, 488, 408);
            ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.current_owner);
            Ov677_ObjArm(gPartyState.current_owner, 0, 0);
            Call3((void (*)())ObjectMotion_ResetAndSetPosition, 14, 504, 408);
            ObjectMotion_CommitCurrentPositionAndActivate(14);
            ObjectMotion_SetAngleToward(14, gPartyState.current_owner, 0);
            Func_02000a80(14);
            if (Func_02000a90()) {
                Ov677_ObjSpeed(14, 0x6666, 0x3333);
                Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 14, 568, 408);
                ObjectMotion_CommitCurrentPositionAndActivate(14);
                Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 14, 568, 360);
                ObjectMotion_CommitCurrentPositionAndActivate(14);
                Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 16, 632, 346);
                Ov677_ObjArm(16, 0x4000, 0);
                Object_SetModeById(16, 0);
                Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 14, 600, 360);
                ObjectMotion_CommitCurrentPositionAndActivate(14);
                Object_SetModeById(14, 0);
                Ov677_ObjArm(14, 0x4000, 0);
                Object_GetById(14)->collision_flags = 129;
                Object_GetById(14)->priority_flags = 1;
                ObjectMotion_SetActionVariant(14, 1);
                Object_GetById(14)->priority_flags |= 1;
#ifndef TLA_EDITION_JA
                Func_020009f8(17, 0, 0);
#endif
                Ov677_FlagSet(0x301);
            } else {
                Call3((void (*)())ObjectMotion_ResetAndSetPosition, 14, 491, 398);
                ObjectMotion_CommitCurrentPositionAndActivate(14);
                Ov677_ObjArm(14, 0x4000, 0);
                ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
                Call2((void (*)())Func_02000a60, 14, 0);
            }
        } else {
            Func_02000a18(message + 2);
            Ov677_MessagePut(14, 0);
        }
    }
}
