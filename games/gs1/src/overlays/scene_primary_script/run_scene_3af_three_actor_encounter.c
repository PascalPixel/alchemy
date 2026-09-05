#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02007768();
void Func_02007770();
s32 Func_02007786();
void Func_020077b2();
void Func_020077ca();
s32 Func_020077d0();
s32 Func_020077e8();
void Func_02007822();
void Func_020078ba();
void Func_020078c8();
void Func_02007f78();
void Func_02007fb6();
s32 Func_02007fc2();
s32 Func_02007fd6();
s32 Func_02007fea();
void Func_02008000();
void Func_02008012();
void Func_02008020();
void Func_0200802a();
void Func_02008030();
void Func_02008044();
void Func_02008058();
void Func_0200806e();
void Func_02008078();
void Func_02008084();
void Func_0200808c();
void Func_020080ac();
void Func_020080b4();
s32 Func_020080b8();
s32 Func_02008104();
void Func_02008106();
void Func_02008122();
void Func_02008126();
void Func_02008132();
void Func_02008138();
void Func_0200813c();
void Func_0200814c();
void Func_02008154();
void Func_02008158();
s32 Func_02008160();
s32 Func_02008164();
void Func_0200816e();
void Func_02008184();
u8 *Scene_GetRecord_5();
void Func_02008188();
s32 Func_02008196();
void Func_020081a0();
void Func_020081a6();
void Func_020081ac();
u8 *Scene_GetRecord_7();
void Func_020081be();
void Func_020081ca();
void Func_020081ce();
void Func_020081d8();
void Func_020081dc();
void Func_020081fa();
void Func_020081fc();
void Func_02008206();
void Func_0200820c();
void Func_02008212();
void Func_0200822c();
void Func_02008236();
void Func_02008248();
void Func_02008292();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02007f78(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02007fb6, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02008000, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02008084, a0, a1, a2)
#define Scene_GetRecord_1(a0) Value1(Func_02007fc2, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02008030(args)
#define Scene_GetRecord_2(a0) Value1(Func_02007fd6, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02008044(args)
#define Scene_GetRecord_3(a0) Value1(Func_02007fea, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_02008058(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02008012, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02008020, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_0200802a, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_0200806e, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Call3(Func_02008078, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_0200808c, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_020080ac(args)
#define Object_SetModeById_2(args...) Func_020080b4(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_0200802a(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02008126, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02008132, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02008122, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02008154(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02008188, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02008138(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1) Value2(Func_02008160, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_020080b8, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_0200813c(args)
#define Object_SetModeById_3(args...) Func_0200814c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02008184(args)
#define ObjectMotion_SetSpeedParameters_6(args...) Func_020081ac(args)
#define UiWork_WaitThenFinalizeCapacity_2(a0, a1) Value2(Func_02008104, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_020081a0(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_020081be, a0)
#define ObjectMotion_SetSpeedParameters_7(args...) Func_020081ce(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02008106(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_020081a6(args)
#define SceneWork_SetStepValue_3(a0) Call1(Func_020081dc, a0)
#define ObjectMotion_SetSpeedParameters_8(a0, a1, a2) Call3(Func_02008160, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_9(a0, a1, a2) Call3(Func_0200816e, a0, a1, a2)
#define Scene_GetRecord_4(args...) Func_02008164(args)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_020081ca, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02008158(args)
#define Scene_GetRecord_5(args...) Func_02008186(args)
#define Scene_GetRecord_6(a0) Value1(Func_02008196, a0)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_020081fa, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02008188(args)
#define Scene_GetRecord_7(args...) Func_020081b6(args)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02008292, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_3(a0, a1, a2) Call3(Func_0200822c, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_4(a0, a1, a2) Call3(Func_02008236, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_5(a0, a1, a2) Call3(Func_02008248, a0, a1, a2)
#define ObjectTable_DestroyById_1(args...) Func_02008206(args)
#define ObjectTable_DestroyById_2(args...) Func_0200820c(args)
#define ObjectTable_DestroyById_3(args...) Func_02008212(args)
#define GameFlag_Set_1(a0) Call1(Func_020081d8, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_020081fc(args)
u8 *Func_02008186();
u8 *Func_020081b6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Offset of a flag byte on an actor record, cleared and set below. */
#define ACTOR_FLAGS_OFFSET 90

/* Sets up actors 1, 2, and 3 from three source records, runs their
 * animations and a wait loop gated on actor 0, then clears a flag byte
 * at +90 on actors 21 and 22 before finishing the scene. */
void FieldScene_RunThreeActorEncounter(void)
{
    u8 *record;

    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedParameters_1(0, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset_1(0, 180, 0x28e);
    ObjectMotion_ArmCallback_1(0, 0x8000, 0);
    /* For each of actors 1, 2, and 3: fetch a source record, and if one
     * exists, copy its fields at +8 and +16 into the actor. */
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_1(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_2(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_3(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    ObjectMotion_SetSpeedParameters_2(1, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_3(2, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters_4(3, 0x13333, 0x9999);
    ObjectMotion_ResetAndSetPositionInMode2_1(1, 194, 0x280);
    ObjectMotion_ResetAndSetPositionInMode2_2(2, 198, 0x28e);
    ObjectMotion_SetPositionAndReset_2(3, 194, 0x2a0);
    Object_SetModeById_1(1, 1);
    Object_SetModeById_2(2, 1);
    BattleRuntime_WaitIfModeZero_1(10);
    ObjectMotion_ArmCallback_2(1, 0x8000, 0);
    ObjectMotion_ArmCallback_3(2, 0x8000, 0);
    Value2(Func_02007768, 3, 0x8000);
    Func_02007770(22, 0);
    SceneWork_SetStepValue_1(0x1f55);
    Call1((void (*)())Func_02007768, 22);
    Value2(Func_02007786, 21, 0xd000);
    BattleRuntime_RunThenWaitIfModeZero_1(21, 0, 40);
    BattleEffect_SpawnLinkedResourceObject_1(22, 0x100, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(22, 1);
    ObjectMotion_SetSpeedParameters_5(22, 0);
    /* Gated on a condition read from actor 0: configure actors 2, 1, and 3,
     * then spin, re-checking actor 0, while a condition on actor 2 holds. */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 1) {
        ObjectMotion_CallThenWaitForAnimationChange_1(2, 4);
        Func_020077b2(2);
        Value2(Func_020077d0, 3, 0xa000);
        Object_SetModeById_3(3, 3);
        Func_020077ca(3);
        Value2(Func_020077e8, 1, 0x6000);
        ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 1);
        ObjectMotion_SetSpeedParameters_6(1, 0);
        L_02003dfa:;
        if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 1) {
            ObjectMotion_SetVariantCallbackAndInvokeObject_3(2, 1);
            SceneWork_SetStepValue_2(0x1f53);
            ObjectMotion_SetSpeedParameters_7(2, 0);
            goto L_02003dfa;
        }
    }
    BattleRuntime_WaitIfModeZero_2(20);
    ObjectMotion_CallThenWaitForAnimationChange_2(22, 3);
    SceneWork_SetStepValue_3(0x1f5b);
    Func_02007822(22);
    ObjectMotion_SetSpeedParameters_8(22, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters_9(21, 0x10000, 0x8000);
    /* Clear the low bit of the flag byte on actor 22. */
    *(u8 *)(Scene_GetRecord_4(22) + ACTOR_FLAGS_OFFSET) &= 254;
    ObjectMotion_SetPositionAndReset_3(22, 162, 0x27a);
    BattleRuntime_WaitIfModeZero_3(1);
    {
        /* Set the low bit of the flag byte on actor 22. */
        u8 *record = Scene_GetRecord_5(22);
        u8 value = *(volatile u8 *)&record[ACTOR_FLAGS_OFFSET];

        record[ACTOR_FLAGS_OFFSET] = (u8)(value | 1);
    }
    /* Clear the low bit of the flag byte on actor 21. */
    *(u8 *)(Scene_GetRecord_6(21) + ACTOR_FLAGS_OFFSET) &= 254;
    ObjectMotion_SetPositionAndReset_4(21, 162, 0x2a4);
    BattleRuntime_WaitIfModeZero_4(1);
    {
        /* Set the low bit of the flag byte on actor 21. */
        u8 *record = Scene_GetRecord_7(21);
        u8 value = *(volatile u8 *)&record[ACTOR_FLAGS_OFFSET];

        record[ACTOR_FLAGS_OFFSET] = (u8)(value | 1);
    }
    ObjectMotion_ArmCallback_4(22, 0x3000, 0);
    Call2(Func_020078c8, 21, 0xd000);
    Func_020078ba(22);
    /* Finish actors 1, 2, and 3 with the same target values used earlier. */
    ObjectMotion_ResetAndSetPositionInMode2_3(1, 180, 0x28e);
    ObjectMotion_ResetAndSetPositionInMode2_4(2, 180, 0x28e);
    ObjectMotion_SetPositionAndReset_5(3, 180, 0x28e);
    ObjectTable_DestroyById_1(1);
    ObjectTable_DestroyById_2(2);
    ObjectTable_DestroyById_3(3);
    GameFlag_Set_1(0x903);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
