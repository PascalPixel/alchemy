#include "types.h"

#define GameFlag_IsSet_1(a0) Value1(Func_02001a8c, a0)
#define GameFlag_IsSet_2(a0) Value1(Func_02001a98, a0)
#define GameFlag_IsSet_3(a0) Value1(Func_02001aa6, a0)
#define GameFlag_Set_1(a0) Call1(Func_02001aba, a0)
#define BattleRuntime_Reset_1(args...) Func_02001ae6(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02001b6c, a0)
#define Scene_GetRecord_1(args...) Func_02001b02(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02001b50(args)
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Call3(Func_02001b7c, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02001b5a, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02001bbe, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02001b74, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02001be0, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02001be4, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02001bb4(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02001be4_a(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02001c06, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02001c10, a0, a1, a2)
#define BattleEvent_RunActionAndWait_2(args...) Func_02001c00(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02001b76(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02001bee(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_02001c16(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02001c32, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_1(args...) Func_02001c32_a(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02001ba0(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02001c54, a0, a1, a2)
#define BattleEvent_RunActionAndWait_4(args...) Func_02001c44(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02001c24(args)
#define BattleEvent_RunActionAndWait_5(args...) Func_02001c54_a(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02001bee_a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02001c24_a, a0, a1, a2)
#define BattleEvent_RunActionAndWait_6(args...) Func_02001c74(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02001c54_b(args)
#define Object_SetModeById_1(args...) Func_02001c54_c(args)
#define Scene_GetRecord_2(args...) Func_02001c12(args)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02001c44_a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02001c62(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02001c74_a(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02001c30(args)
#define ACTOR_ID 13
#define FieldScene_RunMiddleAuxiliarySequence Func_02000464
#define FieldScene_RunActorThirteenRestoration Func_02000838

extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern u8 Data_000000a5[];
extern u8 Data_02009a00[];
extern s16 Data_02000240_t[][1];

s32 Func_020016d4();
s32 Func_020016da();
s32 Func_0200170c();
void Func_02001714();
void Func_02001714_a();
void Func_0200171c();
s32 Func_0200173a();
void Func_02001762();
void Func_0200176c();
void Func_0200177c();
void Func_02001784();
void Func_0200178a();
void Func_0200178e();
void Func_020017aa();
void Func_020017ba();
void Func_020017c4();
void Func_02001840();
s32 Func_02001a8c();
s32 Func_02001a98();
s32 Func_02001aa6();
void Func_02001aba();
void Func_02001ae6();
u8 *Func_02001b02();
void Func_02001b50();
void Func_02001b5a();
void Func_02001b6c();
void Func_02001b74();
void Func_02001b76();
void Func_02001b7c();
void Func_02001ba0();
void Func_02001bb4();
void Func_02001bbe();
void Func_02001be0();
void Func_02001be4();
void Func_02001be4_a();
void Func_02001bee();
void Func_02001bee_a();
void Func_02001c00();
void Func_02001c06();
void Func_02001c10();
u8 *Func_02001c12();
void Func_02001c16();
void Func_02001c24();
void Func_02001c24_a();
void Func_02001c30();
void Func_02001c32();
void Func_02001c32_a();
void Func_02001c44();
void Func_02001c44_a();
void Func_02001c54();
void Func_02001c54_a();
void Func_02001c54_b();
void Func_02001c54_c();
void Func_02001c62();
void Func_02001c74();
void Func_02001c74_a();
void Func_02001fa2();
s32 Func_02001fa8();
void Func_02001fbe();
s32 Func_02001fca();
s32 Func_02001fd2();
s32 Func_02001fd6();
void Func_02002026();
void Func_020020d4();
s32 Func_02002042();
void Func_0200204a();
s32 Func_0200206e();
void Func_02002070();
s32 Func_0200207a();
void Func_020020ce();
void Func_02002180();
void Func_0200218c();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Actor id operated on throughout this scene. */

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Resolved from the main-image function's own source (not registered in the
 * overlay veneer table): each macro names what that source shows the
 * function doing, keeping the site's own calling form. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000838(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02000d24(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value2_02000dc8(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void FieldScene_RunMiddleAuxiliarySequence(s32 a0)
{
    s32 p10;
    s32 rec2;
    u8 *rec7;
    s32 record;
    u8 *p6;
    u8 *base;

    base = Data_02000240;
    p6 = *(volatile s32 *)(base + 500);
    p10 = a0;
    rec7 = Value1(Func_0200170c, (s32)p6);
    Func_02001714(p10);
    rec2 = Value1(Func_020016da, 0x20f);
    if (rec2 == 0) {
        Func_02001714_a();
        Call2(Func_020017c4, (s32)p6, 0x101);
        Func_0200177c((s32)p6, 9);
        record = Value1(Func_0200173a, p10);
        if (record != 0) {
            Func_0200176c((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_0200178a((s32)p6);
        Func_02001840(244);
        Value2(Func_020016d4, 0x2008401, 0xc80);
        rec7[85] = rec2;
        {
            s32 v12 = *(volatile s32 *)((s32)rec7 + 12) + 0x200000;
            Func_0200171c((s32)rec7, *(volatile s32 *)((s32)rec7 + 8), v12, *(volatile s32 *)((s32)rec7 + 16));
        }
        Func_020017ba((s32)p6);
        *(volatile s32 *)((s32)rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        Call1(Func_02001762, 0x20f);
        Call2(Func_02001784, 0x218, p10);
        Call2(Func_0200178e, 0x210, 180);
        Func_020017aa();
        *(u16 *)((*(u8 *volatile *)Data_03001ebc + 0x17c)) = rec2;
    }
}

/* Runs a scene step only when three preceding gate checks all pass;
 * otherwise the function does nothing. Drives actor ACTOR_ID through a
 * long sequence of position/pose/flag updates, applying two record-driven
 * position fixups read back from lookup calls partway through. */
void FieldScene_RunActorThirteenRestoration(void)
{
    u32 i;
    u8 *record;

    if (GameFlag_IsSet_1(0x9a0) == 0) {
    } else {
        if (GameFlag_IsSet_2(0x1b7) != 0) {
        } else {
            if (GameFlag_IsSet_3(0x9b0) == 0) {
            } else {
                GameFlag_Set_1(0x9b5);
                BattleRuntime_Reset_1();
                SceneWork_SetStepValue_1(0x2633);
                /* Record layout observed here: s32 at +8, s32 at +16. */
                record = Scene_GetRecord_1(0);
                if (record != 0) {
                    ObjectMotion_SetHorizontalPositionWithTerrain_1(ACTOR_ID, *(s32 *)(record + 8), *(s32 *)(record + 16));
                }
                ObjectMotion_SetAngleToward_1(ACTOR_ID, 0xc000, 0);
                ObjectMotion_SetPositionAndReset_1(0, 0x1b8, 0x4e8);
                ObjectMotion_ArmCallback_1(ACTOR_ID, 0x4000, 0);
                ObjectMotion_SetPositionAndReset_2(0, 0x1bc, 0x4d8);
                BattleEffect_SpawnLinkedResourceObject_1(0, 0x100, 40);
                ObjectMotion_ArmCallback_2(0, 0x4000, 30);
                ObjectMotion_CallThenWaitForAnimationChange_1(ACTOR_ID, 4);
                BattleEvent_RunActionAndWait_1(ACTOR_ID, 0);
                BattleEffect_SpawnLinkedResourceObject_2(0, 0x105, 60);
                BattleEffect_SpawnLinkedResourceObject_3(ACTOR_ID, 0x105, 60);
                BattleEvent_RunActionAndWait_2(ACTOR_ID, 0);
                BattleRuntime_WaitIfModeZero_1(30);
                ObjectMotion_SetVariantCallbackAndInvokeObject_1(ACTOR_ID, 2);
                BattleEvent_RunActionAndWait_3(ACTOR_ID, 0);
                ObjectMotion_ArmCallback_3(ACTOR_ID, 0xc000, 30);
                BattleEventRuntime_ProcessAction_1(ACTOR_ID, 0);
                BattleRuntime_WaitIfModeZero_2(30);
                BattleEffect_SpawnLinkedResourceObject_4(ACTOR_ID, 0x106, 60);
                BattleEvent_RunActionAndWait_4(ACTOR_ID, 0);
                ObjectMotion_CallThenWaitForAnimationChange_2(ACTOR_ID, 3);
                BattleEvent_RunActionAndWait_5(ACTOR_ID, 0);
                ObjectMotion_SetSpeedParameters_1(ACTOR_ID, 0xb333, 0x5999);
                ObjectMotion_SetPositionAndReset_3(ACTOR_ID, 0x1b8, 0x4e8);
                BattleEvent_RunActionAndWait_6(ACTOR_ID, 0);
                ObjectMotion_CallThenWaitForAnimationChange_3(0, 3);
                Object_SetModeById_1(ACTOR_ID, 2);
                /* Record layout observed here: s16 at +10, s16 at +18. */
                record = Scene_GetRecord_2(0);
                if (record != 0) {
                    ObjectMotion_ResetAndSetPosition_1(ACTOR_ID, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                ObjectMotion_CommitCurrentPositionAndActivate_1(ACTOR_ID);
                ObjectMotion_SetHorizontalPositionWithTerrain_2(ACTOR_ID, 0, 0);
                BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
            }
        }
    }
}

s32 Func_02000d24(s32 a0)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[224][0] == (s32)Data_000000a5) {
        *(u8 *)(Func_02001fca(14) + 35) = 2;
        *(u8 *)(Func_02001fd6(14) + 85) = 3;
        Func_02002026(14, 0, 0);
        Call6(Func_02001fa2, 16, 44, 1, 1, 15, 44);
        Func_020020d4(100, 0, 0);
        Call6(Func_02001fbe, 12, 71, 1, 1, 127, 127);
        Value6(Func_02001fd2, 11, 71, 1, 1, 12, 71);
        record = Value1_02000d24(Func_02001fa8, 0x2008ce5);
        do {
            s32 shown = *(u16 *)Data_02009a00;

            *(volatile u16 *)0x0500019e = shown;
        } while (0);
        return record;
    }
    return a0;
}

s32 Func_02000dc8(s32 a0)
{
    u32 i;
    s32 record;
    s32 v5;

    if (Data_02000240_t[224][0] == (s32)Data_000000a5) {
        *(u8 *)(Func_0200206e(14) + 35) = 2;
        v5 = 0;
        *(u8 *)(Func_0200207a(14) + 85) = v5;
        Call3(Func_020020ce, 14, 0xf80000, 0x2c80000);
        Call6(Func_0200204a, 31, 95, 1, 1, 15, 44);
        Call3(Func_02002180, 100, -1, -1);
        Func_0200218c();
        Call6(Func_02002070, 127, 127, 1, 1, 12, 71);
        record = Value2_02000dc8(Func_02002042, 0x2008ce5, 0xc80);
        return record;
    }
    return a0;
}
