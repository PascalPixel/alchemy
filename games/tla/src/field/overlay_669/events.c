#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "call.h"
#include "text/msg_ids.h"

void Battle_WaitMode0();
void Func_020032a8();
void GameFlag_SetBit();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_ResetAndSetPositionInMode2();
void ObjectMotion_SetPositionAndCommit();
void ObjectMotion_SetSpeedParameters();
struct ObjectRuntime *Object_GetById();
void Object_SetModeById();
void Func_02003148();

void Func_0200008c(void)
{
    s32 v5;
    struct ObjectRuntime *object0;
    s32 v6;
    struct ObjectRuntime *object1;

    Call1((void (*)())GameFlag_SetBit, 0x901);
    v5 = &gPartyState;
    v5 = (v5 + 0x214);
    object0 = Object_GetById(*(s32 *)((u8 *)v5 + 0));
    v6 = 192;
    v6 = (v6 << 11);
    *(s32 *)((u8 *)object0 + 40) = v6;
    Call3((void (*)())ObjectMotion_SetSpeedParameters, *(s32 *)((u8 *)v5 + 0), 0x20000, 0x10000);
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, *(s32 *)((u8 *)v5 + 0), 136, 0x1e8);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())ObjectMotion_ResetAndSetPosition, 15, 120, 0x1d8);
    Call3((void (*)())ObjectMotion_SetPositionAndCommit, 14, 136, 0x1db);
    object1 = Object_GetById(15);
    *(s32 *)((u8 *)object1 + 40) = v6;
    Call1((void (*)())Func_020032a8, 0x237);
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, *(s32 *)((u8 *)v5 + 0));
    Call2((void (*)())Object_SetModeById, *(s32 *)((u8 *)v5 + 0), 1);
    Call3((void (*)())ObjectMotion_ArmCallback, *(s32 *)((u8 *)v5 + 0), 0xc000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 15);
    Call1((void (*)())Func_020032a8, 159);
    return;
}

void Func_02000128(s32 arg0)
{
    Call3((void (*)())Func_02003148, arg0, 0x1c9, 0x902);
    return;
}
