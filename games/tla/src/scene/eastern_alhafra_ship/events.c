#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
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

void Battle_WaitMode0();
void Field_BeginPaletteTransition();
void Func_02003130();
void Func_02003138();
void Func_020031c8();
void Func_020031d8();
void Func_020031e0();
void Func_02003240();
void Func_02003268();
void Func_02003278();
void Func_02003280();
void Func_02003288();
void Func_02003290();
void Func_020032a8();
s32 GameFlag_Test();
void GameFlag_SetBit();
void Motion_SetVarCbAndRefresh();
void ObjectMotion_ArmCallback();
struct ObjectRuntime *Object_GetById();

void Func_0200013c(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(12);
    u8 *flags;
    s32 i;

    Func_02003130();
    Call1((void (*)())Func_02003240, 0);
    Call1((void (*)())Func_020031c8, (s32)MsgGooGooGa);
    Call2((void (*)())Func_020031e0, 11, 0);
    Call2((void (*)())Func_02003268, 8, 0x1210);
    Func_02003278();
    Call2((void (*)())Func_02003290, (s32)Object_GetById(id), 2);
    Call1((void (*)())Func_020032a8, 201);
    Call1((void (*)())Battle_WaitMode0, 30);
    flags = &object->motion_flags;
    *flags = 4;
    for (i = 39; i >= 0; i--) {
        s32 y = object->y;

        /* FAKEMATCH: the native loads y before it builds the step (differ 2 without the barrier). */
        __asm__ volatile("" : "+r"(y));
        object->y = y + 0x3333;
        Call1((void (*)())Battle_WaitMode0, 1);
    }
    Call1((void (*)())Battle_WaitMode0, 70);
    Call2((void (*)())Func_02003290, (s32)Object_GetById(id), 0);
    Func_02003288();
    Func_02003280();
    Call1((void (*)())Field_BeginPaletteTransition, 8);
    *flags = 3;
    object->gravity = 0x8000;
    Call1((void (*)())Battle_WaitMode0, 5);
    Call1((void (*)())Func_020032a8, 132);
    if (Value1((s32 (*)())GameFlag_Test, 0x8ae) == 0) {
        Call1((void (*)())GameFlag_SetBit, 0x8ae);
        Call1((void (*)())Battle_WaitMode0, 30);
        Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x3000, 0);
        Call1((void (*)())Battle_WaitMode0, 30);
        Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
        Call1((void (*)())Battle_WaitMode0, 10);
        Call3((void (*)())Func_020031d8, 10, 0, 10);
        Call3((void (*)())ObjectMotion_ArmCallback, 10, 0xafff, 0);
    }
    Func_02003138();
}
