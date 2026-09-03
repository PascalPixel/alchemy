#include "types.h"

#define FieldScene_RunActorNinePresentationCycles Func_020007b0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200126a();
void Func_02001270();
void Func_02001276();
void Func_0200127c();
void Func_02001282();
void Func_02001288();
void Func_0200128e();
void Func_02001376();
void Func_02001476();
void Func_02001988();
void Func_02001994();
void Func_020019c2();
void Func_020019ce();
u8 *Scene_GetRecord_1();
void Func_020019fe();
void Func_02001a3e();
void Func_02001a46();
void Func_02001a5a();
void Func_02001a5e();
void Func_02001a68();
u8 *Scene_GetRecord_2();
void Func_02001a74();
void Func_02001a7c();
void Func_02001a82();
void Func_02001a8a();
void Func_02001a8c();
void Func_02001a98();
void Func_02001aa0();
void Func_02001aa6();
void Func_02001b04();
void Func_02001b0e();
void Func_02001b12();
void Func_02001b14();
void Func_02001b24();
void Func_02001b28();
void Func_02001b2e();
void Func_02001b32();
void Func_02001b44();
void Func_02001b48();
void Func_02001b56();
void Func_02001b58();
void Func_02001b5e();
void Func_02001b6c();
void Func_02001b70();
void Func_02001b74();
void Func_02001b7c();
void Func_02001b82();
void Func_02001b8a();
void Func_02001b90();
void Func_02001b98();
void Func_02001ba0();
void Func_02001c0e();
void Func_02001c18();
void Func_02001c28();
void Func_02001c4e();
void Func_02001c68();
void Func_02001c70();
void Func_02001c72();
void Func_02001c78();
void Func_02001c8a();
void Func_02001c98();
void Func_02001ca6();
void Func_02001cb8();
void Func_02001cc6();
void Func_02001cdc();
void Func_02001d18();
void Func_02001d2c();
void Func_02001d42();
void Func_02001d4e();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_020019ce(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_02001a46(args)
#define Scene_GetRecord_1(args...) Func_020019fc(args)
#define Object_NotifyLastActiveOfEvent_1(a0) Call1(Func_020019fe, a0)
#define ObjectMotion_EnableActionAndSetCallback_1(args...) Func_02001a5a(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02001b14(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_02001b28(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02001a3e(args)
#define Scene_GetRecord_2(args...) Func_02001a6c(args)
#define ObjectMotion_EnableActionAndResetMotion_1(args...) Func_02001a8c(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02001a68(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02001aa6, a0, a1, a2)
#define Object_SetPosition_1(a0, a1, a2, a3) Call4(Func_02001a5e, a0, a1, a2, a3)
#define Object_CommitPosition_1(args...) Func_02001a6c(args)
#define Object_SetPosition_2(a0, a1, a2, a3) Call4(Func_02001a74, a0, a1, a2, a3)
#define Object_CommitPosition_2(args...) Func_02001a82(args)
#define Object_SetPosition_3(a0, a1, a2, a3) Call4(Func_02001a8a, a0, a1, a2, a3)
#define Object_CommitPosition_3(args...) Func_02001a98(args)
#define Object_SetPosition_4(a0, a1, a2, a3) Call4(Func_02001aa0, a0, a1, a2, a3)
#define ObjectMotion_EnableActionAndResetMotion_2(args...) Func_02001b0e(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02001b70, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02001b8a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02001b28, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02001b04(args)
#define ObjectMotion_EnableActionAndSetCallback_2(args...) Func_02001b44(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02001b12(args)
#define ObjectMotion_EnableActionAndResetMotion_3(args...) Func_02001b58(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02001b2e(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02001b6c, a0, a1, a2)
#define Object_SetPosition_5(a0, a1, a2, a3) Call4(Func_02001b24, a0, a1, a2, a3)
#define Object_CommitPosition_4(args...) Func_02001b32(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02001b90, a0, a1, a2)
#define Object_SetPosition_6(a0, a1, a2, a3) Call4(Func_02001b48, a0, a1, a2, a3)
#define Object_CommitPosition_5(args...) Func_02001b56(args)
#define Object_SetPosition_7(a0, a1, a2, a3) Call4(Func_02001b5e, a0, a1, a2, a3)
#define Object_CommitPosition_6(args...) Func_02001b6c(args)
#define Object_SetPosition_8(a0, a1, a2, a3) Call4(Func_02001b74, a0, a1, a2, a3)
#define Object_CommitPosition_7(args...) Func_02001b82(args)
#define Object_SetPosition_9(a0, a1, a2, a3) Call4(Func_02001b8a, a0, a1, a2, a3)
#define Object_CommitPosition_8(args...) Func_02001b98(args)
#define Object_SetPosition_10(a0, a1, a2, a3) Call4(Func_02001ba0, a0, a1, a2, a3)
#define ObjectMotion_EnableActionAndResetMotion_4(args...) Func_02001c0e(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02001c70, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02001c8a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_02001c28, a0, a1, a2)
#define ObjectMotion_Launch_1(args...) Func_02001c68(args)
#define ObjectMotion_Launch_2(args...) Func_02001c72(args)
#define Audio_PlayCue_1(args...) Func_02001d18(args)
#define GameFlag_Set_1(a0) Call1(Func_02001c18, a0)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02001ca6, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02001cb8(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02001cdc, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02001c98, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02001c4e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02001cc6(args)
#define ObjectMotion_SetSpeedParameters_7(args...) Func_02001d4e(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02001c78(args)
u8 *Func_020019fc();
u8 *Func_02001a6c();
u8 *Func_02001a6c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))

/* A record's four-byte field at the given byte offset. Used below for the
 * seven fields that get the same reset pattern twice, once per record. */
#define RECORD_S32(rec, off) (*(s32 *)((rec) + (off)))

/* Runs the record-8/record-9 pair through two near-identical setup-then-move
 * cycles (position waypoints, a movement flag reset, then animation/sound
 * calls), followed by a shorter closing cycle for record 8 alone. */
void FieldScene_RunActorNinePresentationCycles(void)
{
    u32 i;
    u8 *rec9;
    u8 *record;

    BattleRuntime_Reset_1();
    ObjectGroup_ConfigureChildValue_1(0, 15);
    record = Scene_GetRecord_1(0);
    Func_020019c2(record, 0);
    Func_02001988(1);
    Object_NotifyLastActiveOfEvent_1(0x200976c);
    Func_02001994(1);
    Func_0200126a(9);
    Func_02001270(10);
    Func_02001276(11);
    Func_0200127c(12);
    Func_02001282(13);
    Func_02001288(14);
    Func_0200128e(15);
    ObjectMotion_EnableActionAndSetCallback_1(8, 0x200939c);
    SCENE_PHASE = 0x203;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1();
    BattleRuntime_WaitIfModeZero_2(120);
    rec9 = Scene_GetRecord_2(9);
    ObjectMotion_EnableActionAndResetMotion_1(9);
    /* Reset record 9's waypoint/velocity fields: three fields to the
     * minimum s32, then four fields to zero. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    BattleRuntime_WaitIfModeZero_3(20);
    ObjectMotion_SetSpeedParameters_2(9, 0x80000, 0x40000);
    Object_SetPosition_1(rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition_1(rec9);
    Object_SetPosition_2(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_2(rec9);
    Object_SetPosition_3(rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition_3(rec9);
    Object_SetPosition_4(rec9, 0x900000, 0, 0xa90000);
    ObjectMotion_EnableActionAndResetMotion_2(8);
    Func_02001a7c(1);
    ObjectMotion_ArmCallback_1(8, 0x8000, 0);
    BattleEffect_SpawnLinkedResourceObject_1(8, 0x103, 60);
    ObjectMotion_SetSpeedParameters_3(9, 0x20000, 0x10000);
    Func_02001376(9);
    BattleRuntime_WaitIfModeZero_4(20);
    ObjectMotion_EnableActionAndSetCallback_2(8, 0x200939c);
    BattleRuntime_WaitIfModeZero_5(120);
    ObjectMotion_EnableActionAndResetMotion_3(9);
    /* Same reset pattern on record 9 for the second cycle. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    BattleRuntime_WaitIfModeZero_6(20);
    ObjectMotion_SetSpeedParameters_4(9, 0x80000, 0x40000);
    Object_SetPosition_5(rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition_4(rec9);
    ObjectMotion_SetSpeedParameters_5(9, 0x50000, 0x28000);
    Object_SetPosition_6(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_5(rec9);
    Object_SetPosition_7(rec9, 0xa40000, 0x720000, 0x1410000);
    Object_CommitPosition_6(rec9);
    Object_SetPosition_8(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_7(rec9);
    Object_SetPosition_9(rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition_8(rec9);
    Object_SetPosition_10(rec9, 0x900000, 0, 0xa90000);
    ObjectMotion_EnableActionAndResetMotion_4(8);
    Func_02001b7c(1);
    ObjectMotion_ArmCallback_2(8, 0x8000, 0);
    BattleEffect_SpawnLinkedResourceObject_2(8, 0x103, 60);
    ObjectMotion_SetSpeedParameters_6(9, 0x20000, 0x10000);
    Func_02001476(9);
    ObjectMotion_Launch_1(8, 4, 20);
    ObjectMotion_Launch_2(8, 6, 40);
    Audio_PlayCue_1(29);
    GameFlag_Set_1(0x8f0);
    SceneWork_SetStepValue_1(0x1e49);
    BattleRuntime_RunThenWaitIfModeZero_1(16, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_3(8, 0x100, 0);
    ObjectMotion_SetPositionAndReset_1(8, 164, 0x158);
    BattleRuntime_WaitIfModeZero_7(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 2);
    Func_02001d42();
    ObjectMotion_SetSpeedParameters_7();
    Func_02001d2c(12);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
