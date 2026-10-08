#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "call.h"
#include "text/msg_ids.h"

void Battle_WaitMode0();
void Func_02001d50();
void Func_02001d58();
void Func_02001db8();
void Func_02001e18();
void Func_02001e28();
void Func_02001e40();
void Func_02001e48();
void Func_02001e50();
void Func_02001eb8();
void Func_02001ec8();
void Func_02001ef8();
void GameFlag_SetBit();
void GameFlag_Test();
void Inventory_PromptAndSetObjectMode();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_Launch();
void ObjectMotion_OffsetPositionAndReset();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_SetAngleToward();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_SetVariantCallback();
struct ObjectRuntime *Object_GetById();
void Object_LinkPair();
void Object_SetModeById();
void UiText_OpenMessageAtObject();
void Func_02001910(void);
void Func_02001a20(void);

void Func_02001310(void)
{
    struct ObjectRuntime *object0;

    Call1((void (*)())GameFlag_SetBit, 0x210);
    ((void (*)())Func_02001d50)();
    Call1((void (*)())Func_02001eb8, 0);
    object0 = Object_GetById(4);
    /* FAKEMATCH: ⚓️ loads z before it builds the bound. */
    __asm__ volatile("" : "+l"(object0));
    if (object0->z > 0x5f0000)
        Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, object0->x / 0x10000, 95);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, 0x16a, 95);
    Call2((void (*)())Func_02001e40, 4, 0x4000);
    Call1((void (*)())Func_02001e18, (s32)MsgScene659No110);
    Call3((void (*)())ObjectMotion_SetAngleToward, 10, 4, 0);
    Call1((void (*)())Battle_WaitMode0, 30);
    Value3((s32 (*)())Func_02001e48, 10, 0x102, 30);
    Call3((void (*)())Func_02001e28, 10, 0, 5);
    Value3((s32 (*)())Func_02001e48, 11, 0x100, 3);
    Call3((void (*)())ObjectMotion_SetAngleToward, 11, 4, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 11, 0, 5);
    Call1((void (*)())Func_02001e18, (s32)MsgScene659No115);
    Value3((s32 (*)())Func_02001e48, 10, 0x100, 30);
    Call2((void (*)())Func_02001e40, 10, 0);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())Func_02001e28, 10, 0, 5);
    Call3((void (*)())Object_LinkPair, 10, 11, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_SetAngleToward, 10, 4, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Value3((s32 (*)())Func_02001e48, 10, 0x105, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 10, 0, 5);
    Call3((void (*)())ObjectMotion_SetAngleToward, 11, 4, 0);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 11, 2);
    Call1((void (*)())Battle_WaitMode0, 5);
    Call3((void (*)())Func_02001e28, 11, 0, 5);
    ((void (*)())Func_02001d58)();
    return;
}

void Func_02001450(void)
{
    s32 result0;
    s32 result1;
    struct ObjectRuntime *object2;
    struct ObjectRuntime *object3;
    struct ObjectRuntime *object4;
    struct ObjectRuntime *object5;

    ((void (*)())Func_02001d50)();
    Call1((void (*)())Func_02001eb8, 0);
    Call1((void (*)())Func_02001e18, (s32)MsgScene659No126);
    Value3((s32 (*)())Func_02001e48, 28, 0x100, 30);
    Call3((void (*)())ObjectMotion_SetAngleToward, 28, 4, 0);
    Call3((void (*)())Func_02001e28, 28, 0, 10);
    Call3((void (*)())ObjectMotion_SetAngleToward, 15, 4, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 17, 0xb000, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 18, 4, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 14, 4, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 15, 4, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 16, 4, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, 0x2bd, 93);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 4, -32, 0);
    Call4((void (*)())Func_02001ec8, 5, -8, 32, 0x8000);
    Call4((void (*)())Func_02001ec8, 31, -16, 0, 0x5000);
    Call4((void (*)())Func_02001ec8, 6, -8, 16, 0x6000);
    Call4((void (*)())Func_02001ec8, 7, -24, 16, 0x6000);
    Call1((void (*)())Battle_WaitMode0, 30);
    Value3((s32 (*)())Func_02001e48, 15, 0x100, 30);
    Call3((void (*)())Func_02001e28, 15, 0, 10);
    Value3((s32 (*)())Func_02001e48, 27, 0x100, 30);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 27, 0, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 27, 2);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 27, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Value3((s32 (*)())Func_02001e48, 27, 0x109, 30);
    Call3((void (*)())Func_02001e28, 27, 0, 10);
    Call3((void (*)())Object_LinkPair, 7, 4, 0);
    Call1((void (*)())Battle_WaitMode0, 30);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x8000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0x6000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call3((void (*)())ObjectMotion_Launch, 18, 6, 20);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 18, 0, 10);
    Call2((void (*)())Func_02001e40, 7, 0x3000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Value3((s32 (*)())Func_02001e48, 28, 0x102, 30);
    Value2((s32 (*)())UiText_OpenMessageAtObject, 28, 0);
    result0 = Value2((s32 (*)())Inventory_PromptAndSetObjectMode, 4, 0);
    if (result0 != 0) goto L_02009690;
    Value3((s32 (*)())Func_02001e48, 27, 0x108, 30);
    Call3((void (*)())Func_02001e28, 27, 0, 10);
    Call2((void (*)())Func_02001e40, 7, 0x6000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Value3((s32 (*)())Func_02001e48, 15, 0x108, 30);
    Call3((void (*)())Func_02001e28, 15, 0, 10);
    ((struct EventWork *)Ram_HeapSlots->event_work)->message_id += 3;
    goto L_02009700;
L_02009690:
    Value3((s32 (*)())Func_02001e48, 15, 0x102, 30);
    ((void (*)())Func_02001ef8)();
    ((struct EventWork *)Ram_HeapSlots->event_work)->message_id += 3;
    Call3((void (*)())Func_02001e28, 27, 0, 10);
    Call2((void (*)())Func_02001e40, 7, 0x6000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call2((void (*)())ObjectMotion_SetVariantCallback, 15, 3);
    Call2((void (*)())Func_02001e50, 15, 0x102);
    Call1((void (*)())Battle_WaitMode0, 50);
    Call3((void (*)())Func_02001e28, 15, 0, 10);
L_02009700:
    Call2((void (*)())Object_SetModeById, 7, 3);
    Call2((void (*)())Object_SetModeById, 6, 3);
    Call2((void (*)())Object_SetModeById, 31, 3);
    Call2((void (*)())Object_SetModeById, 5, 3);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 31, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 31, 0, 10);
    result1 = Value1((s32 (*)())GameFlag_Test, 0x981);
    if (result1 == 0) goto L_02009760;
    ((void (*)())Func_02001910)();
    goto L_02009764;
L_02009760:
    ((void (*)())Func_02001a20)();
L_02009764:
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x4000, 0);
    Call2((void (*)())Func_02001e40, 4, 0x4000);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Object_SetModeById, 7, 3);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
    Call1((void (*)())Battle_WaitMode0, 40);
    Call3((void (*)())ObjectMotion_ArmCallback, 5, 0xa000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 6, 0x8000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 31, 0x6000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0x2000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0, 0);
    Call1((void (*)())Battle_WaitMode0, 40);
    Call2((void (*)())Object_SetModeById, 5, 3);
    Call2((void (*)())Object_SetModeById, 6, 3);
    Call2((void (*)())Object_SetModeById, 31, 3);
    Call2((void (*)())Object_SetModeById, 7, 3);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 31, 0x13333, 0x9999);
    Call2((void (*)())Object_SetModeById, 31, 2);
    object2 = Object_GetById(4);
    if (object2 == 0) goto L_02009832;
    Value3((s32 (*)())ObjectMotion_ResetAndSetPosition, 31, *(s16 *)((u8 *)object2 + 10), *(s16 *)((u8 *)object2 + 18));
L_02009832:
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 31);
    Call3((void (*)())Func_02001db8, 31, 0, 0);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 6, 0x13333, 0x9999);
    Call2((void (*)())Object_SetModeById, 6, 2);
    object3 = Object_GetById(4);
    if (object3 == 0) goto L_02009870;
    Value3((s32 (*)())ObjectMotion_ResetAndSetPosition, 6, *(s16 *)((u8 *)object3 + 10), *(s16 *)((u8 *)object3 + 18));
L_02009870:
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 6);
    Call3((void (*)())Func_02001db8, 6, 0, 0);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 7, 0x13333, 0x9999);
    Call2((void (*)())Object_SetModeById, 7, 2);
    object4 = Object_GetById(4);
    if (object4 == 0) goto L_020098ae;
    Value3((s32 (*)())ObjectMotion_ResetAndSetPosition, 7, *(s16 *)((u8 *)object4 + 10), *(s16 *)((u8 *)object4 + 18));
L_020098ae:
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 7);
    Call3((void (*)())Func_02001db8, 7, 0, 0);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 5, 0x13333, 0x9999);
    Call2((void (*)())Object_SetModeById, 5, 2);
    object5 = Object_GetById(4);
    if (object5 == 0) goto L_020098ec;
    Value3((s32 (*)())ObjectMotion_ResetAndSetPosition, 5, *(s16 *)((u8 *)object5 + 10), *(s16 *)((u8 *)object5 + 18));
L_020098ec:
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 5);
    Call3((void (*)())Func_02001db8, 5, 0, 0);
    Call1((void (*)())GameFlag_SetBit, 0x989);
    ((void (*)())Func_02001d58)();
    return;
}

void Func_02001910(void)
{
    Call1((void (*)())Func_02001e18, (s32)MsgScene659No127);
    Call3((void (*)())Func_02001e28, 28, 0, 10);
    Call3((void (*)())Object_LinkPair, 4, 31, 0);
    Call1((void (*)())Battle_WaitMode0, 40);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0x6000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 31, 0x5000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Value3((s32 (*)())Func_02001e48, 15, 0x100, 30);
    Call3((void (*)())Func_02001e28, 15, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Value3((s32 (*)())Func_02001e48, 27, 0x102, 30);
    Call3((void (*)())Func_02001e28, 27, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 31, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 31, 0, 10);
    Value3((s32 (*)())Func_02001e48, 5, 0x108, 30);
    Call3((void (*)())Func_02001e28, 5, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 17, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 17, 0, 10);
    Call2((void (*)())Func_02001e40, 5, 0x4000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 18, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 18, 0, 10);
    return;
}

void Func_02001a20(void)
{
    Call1((void (*)())Func_02001e18, (s32)MsgScene659No128);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 17, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 17, 0, 10);
    Call2((void (*)())Func_02001e40, 5, 0x4000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 4);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 5, 0, 10);
    Call3((void (*)())ObjectMotion_SetAngleToward, 6, 5, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 6, 0, 10);
    Call2((void (*)())Func_02001e40, 5, 0xc000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Value3((s32 (*)())Func_02001e48, 17, 0x101, 30);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 17, 0);
    Value3((s32 (*)())Func_02001e48, 7, 0x108, 30);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Value3((s32 (*)())Func_02001e48, 31, 0x102, 30);
    Call3((void (*)())ObjectMotion_SetAngleToward, 31, 7, 0);
    Call3((void (*)())Func_02001e28, 31, 0, 10);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 31, 0);
    Call1((void (*)())Battle_WaitMode0, 60);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call3((void (*)())ObjectMotion_SetAngleToward, 4, 7, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 6, 7, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 5, 7, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Value3((s32 (*)())Func_02001e48, 6, 0x101, 30);
    Call3((void (*)())Func_02001e28, 6, 0, 10);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 6, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Value3((s32 (*)())Func_02001e48, 4, 0x101, 30);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 4, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Value3((s32 (*)())Func_02001e48, 5, 0x109, 30);
    Call3((void (*)())Func_02001e28, 5, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Func_02001e40, 7, 0x2000);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 31, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 31, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 4, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call1((void (*)())GameFlag_SetBit, 0x981);
    Value3((s32 (*)())Func_02001e48, 15, 0x100, 30);
    Call3((void (*)())Func_02001e28, 15, 0, 10);
    Call2((void (*)())Func_02001e40, 7, 0x6000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0x6000, 0);
    Value3((s32 (*)())Func_02001e48, 27, 0x102, 30);
    Call3((void (*)())Func_02001e28, 27, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 31, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 31, 0, 10);
    Value3((s32 (*)())Func_02001e48, 5, 0x108, 30);
    Call3((void (*)())ObjectMotion_ArmCallback, 5, 0x8000, 0);
    Call3((void (*)())Func_02001e28, 5, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 17, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 17, 0, 10);
    Call2((void (*)())Func_02001e40, 5, 0x4000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 18, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 18, 0, 10);
    return;
}
