#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_020015b8();
void Func_020015be();
void Func_020015c4();
void Func_020015ca();
void Func_020015d0();
void Func_020015d6();
void Func_020015dc();
void Func_02001cca();
void Func_02001cd6();
void Func_02001ce2();
void Func_02001d04();
void Func_02001d10();
s32 Func_02001d3e();
void Func_02001d40();
void Func_02001d4c();
void Func_02001d88();
void Func_02001d8a();
void Func_02001d96();
void Func_02001da4();
void Func_02001ddc();
void Func_02001de2();
void Func_02001de8();
void Func_02001dee();
void Func_02001df4();
void Func_02001dfa();
void Func_02001dfe();
void Func_02001e00();
void Func_02001e0c();
void Func_02001e1a();
void Func_02001e28();
void Func_02001e36();
void Func_02001e44();
void Func_02001e52();
void Func_02001e5e();
void Func_02001e72();
void Func_02001e74();
void Func_02001e7e();
void Func_02001e82();
void Func_02001e88();
void Func_02001e90();
void Func_02001e92();
void Func_02001e9c();
void Func_02001e9e();
void Func_02001ea8();
void Func_02001eb4();
void Func_02001ece();
void Func_02001ed8();
void Func_02001ee4();
void Func_02001eee();
void Func_02001ef8();
s32 Func_02001efc();
void Func_02001f02();
void Func_02001f0c();
void Func_02001f10();
void Func_02001f16();
void Func_02001f1c();
void Func_02001f20();
void Func_02001f30();
void Func_02001f42();
void Func_02001f6e();
s32 Func_02001f98();
void Func_02001f9a();
void Func_02001f9e();
void Func_02001fa2();
void Func_02001fae();
void Func_02001fb4();
void Func_02001fb6();
void Func_02001fc2();
void Func_02001fde();
void Func_02001ff4();
void Func_02001ffa();
void Func_02002008();
void Func_0200200a();
void Func_02002010();
void Func_02002030();
void Func_02002040();
s32 Func_02002042();
void Func_0200204c();
void Func_02002060();
void Func_02002062();
void Func_0200206c();
void Func_020020ae();
void Func_020020e6();
void Func_02002114();
void Func_0200212a();
void Func_02002136();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02001d10(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_02001d88(args)
#define Scene_GetRecord_1(args...) Func_02001d3e(args)
#define Object_NotifyLastActiveOfEvent_1(a0) Call1(Func_02001d40, a0)
#define Object_NotifyLastActiveOfEvent_2(a0) Call1(Func_02001d4c, a0)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Call2(Func_02001da4, a0, a1)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02001e5e(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_02001e72(args)
#define BattleRuntime_WaitIfModeZero_2(a0) Call1(Func_02001d8a, a0)
#define Audio_PlayCue_1(args...) Func_02001e90(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02001d96(args)
#define ObjectMotion_EnableActionAndResetMotion_1(args...) Func_02001ddc(args)
#define ObjectMotion_EnableActionAndResetMotion_2(args...) Func_02001de2(args)
#define ObjectMotion_EnableActionAndResetMotion_3(args...) Func_02001de8(args)
#define ObjectMotion_EnableActionAndResetMotion_4(args...) Func_02001dee(args)
#define ObjectMotion_EnableActionAndResetMotion_5(args...) Func_02001df4(args)
#define ObjectMotion_EnableActionAndResetMotion_6(args...) Func_02001dfa(args)
#define ObjectMotion_EnableActionAndResetMotion_7(args...) Func_02001e00(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02001dfe, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02001e0c, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02001e1a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02001e28, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_02001e36, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_7(a0, a1, a2) Call3(Func_02001e44, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_8(a0, a1, a2) Call3(Func_02001e52, a0, a1, a2)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02001e74(args)
#define ObjectMotion_ResetAndSetPosition_2(args...) Func_02001e7e(args)
#define ObjectMotion_ResetAndSetPosition_3(args...) Func_02001e88(args)
#define ObjectMotion_ResetAndSetPosition_4(args...) Func_02001e92(args)
#define ObjectMotion_ResetAndSetPosition_5(args...) Func_02001e9c(args)
#define ObjectMotion_ResetAndSetPosition_6(a0, a1, a2) Call3(Func_02001ea8, a0, a1, a2)
#define ObjectMotion_ResetAndSetPosition_7(a0, a1, a2) Call3(Func_02001eb4, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02001e72(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02001f1c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02001e82(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02001ee4(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02001eee(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_02001ef8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_02001f02(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(args...) Func_02001f0c(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(args...) Func_02001f16(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_7(args...) Func_02001f20(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02001ece(args)
#define Scene_GetRecord_2(args...) Func_02001efc(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_8(a0, a1, a2) Call3(Func_02001f42, a0, a1, a2)
#define ObjectMotion_EnableActionAndResetMotion_8(args...) Func_02001f30(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02001f6e(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02001f9a, a0, a1, a2)
#define Audio_PlayCue_2(args...) Func_02002010(args)
#define GameFlag_Set_1(a0) Call1(Func_02001f10, a0)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02001fde, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02001fb6(args)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02001fa2, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02001fae, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02001ffa, a0, a1, a2)
#define Scene_GetRecord_3(args...) Func_02001f98(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02001f9e(args)
#define ObjectMotion_Launch_1(args...) Func_02002008(args)
#define Audio_PlayCue_3(args...) Func_020020ae(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02001fb4(args)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Call2(Func_02001ff4, a0, a1)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02001fc2(args)
#define ObjectMotion_SetActionVariant_1(args...) Func_02002062(args)
#define ObjectMotion_SetSpeedParameters_9(a0, a1, a2) Call3(Func_02002008, a0, a1, a2)
#define Audio_PlayCue_4(args...) Func_020020e6(args)
#define ObjectMotion_ResetAndSetPosition_8(a0, a1, a2) Call3(Func_02002040, a0, a1, a2)
#define ObjectMotion_ResetAndSetPosition_9(a0, a1, a2) Call3(Func_0200204c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_0200200a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_9(args...) Func_0200206c(args)
#define Scene_GetRecord_4(args...) Func_02002042(args)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_02002030(args)
#define ObjectMotion_SetSpeedParameters_10(args...) Func_02002136(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02002060(args)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
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

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))

/* Drives ids 8 through 18 through position, scale, and flag updates in
 * sequence, stepping the shared scene phase at the start and end. */
void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 rec;
    s32 id0_state;

    BattleRuntime_Reset_1();
    ObjectGroup_ConfigureChildValue_1(0, 15);
    id0_state = Scene_GetRecord_1(0);
    Func_02001d04(id0_state, 0);
    Func_02001cca(1);
    Object_NotifyLastActiveOfEvent_1(0x200976c);
    Func_02001cd6(1);
    Object_NotifyLastActiveOfEvent_2(0x2009844);
    Func_02001ce2(1);
    Func_020015b8(9);
    Func_020015be(10);
    Func_020015c4(11);
    Func_020015ca(12);
    Func_020015d0(13);
    Func_020015d6(14);
    Func_020015dc(15);
    ObjectMotion_EnableActionAndSetCallback_1(8, 0x200939c);
    SCENE_PHASE = 0x203;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1();
    BattleRuntime_WaitIfModeZero_2(0x12c);
    Audio_PlayCue_1(147);
    BattleRuntime_WaitIfModeZero_3(100);
    ObjectMotion_EnableActionAndResetMotion_1(9);
    ObjectMotion_EnableActionAndResetMotion_2(10);
    ObjectMotion_EnableActionAndResetMotion_3(11);
    ObjectMotion_EnableActionAndResetMotion_4(12);
    ObjectMotion_EnableActionAndResetMotion_5(13);
    ObjectMotion_EnableActionAndResetMotion_6(14);
    ObjectMotion_EnableActionAndResetMotion_7(15);
    ObjectMotion_SetSpeedParameters_2(9, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_3(10, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_4(11, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_5(12, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_6(13, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_7(14, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_8(15, 0x30000, 0x18000);
    ObjectMotion_ResetAndSetPosition_1(9, 0, 100);
    ObjectMotion_ResetAndSetPosition_2(10, 60, 100);
    ObjectMotion_ResetAndSetPosition_3(11, 120, 100);
    ObjectMotion_ResetAndSetPosition_4(12, 180, 100);
    ObjectMotion_ResetAndSetPosition_5(13, 240, 100);
    ObjectMotion_ResetAndSetPosition_6(14, 0x140, 100);
    ObjectMotion_ResetAndSetPosition_7(15, 0x17c, 100);
    BattleRuntime_WaitIfModeZero_4(40);
    BattleEffect_SpawnLinkedResourceObject_1(8, 0x101, 0);
    BattleRuntime_WaitIfModeZero_5(20);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(9, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(10, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(11, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(12, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(13, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(14, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_7(15, 0, 0);
    BattleRuntime_WaitIfModeZero_6(100);
    rec = Scene_GetRecord_2(18);
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    ObjectMotion_SetHorizontalPositionWithTerrain_8(18, 0xac0000, 0x1540000);
    ObjectMotion_EnableActionAndResetMotion_8(8);
    Func_02001e9e(1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 1);
    ObjectMotion_ArmCallback_1(8, 0x3000, 0);
    Audio_PlayCue_2(29);
    GameFlag_Set_1(0x8f0);
    for (i = 0; i < 32; i++) {
        *(s32 *)(rec + 24) += 0xccc;
        *(s32 *)(rec + 28) += 0xccc;
        Func_02001ed8(1);
    }
    BattleEffect_SpawnLinkedResourceObject_2(8, 0x101, 60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(8, 2);
    ObjectMotion_SetPositionAndReset_1(8, 168, 0x154);
    ObjectMotion_SetPositionAndReset_2(8, 200, 0x154);
    ObjectMotion_ArmCallback_2(8, 0x8000, 0);
    rec = Scene_GetRecord_3(17);
    *(s32 *)(rec + 24) = 0x12666;
    *(s32 *)(rec + 28) = 0x12666;
    *(s32 *)(rec + 8) = 0xac0000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x1540000;
    {
        /* Clear the flag word at +6. */
        s32 shown = 0;

        *(u16 *)(rec + 6) = shown;
    }
    *(s32 *)(rec + 68) = 0x6666;
    *(s32 *)(rec + 72) = 0x30000;
    BattleRuntime_WaitIfModeZero_7(20);
    ObjectMotion_Launch_1(8, 6, 20);
    Audio_PlayCue_3(147);
    BattleRuntime_WaitIfModeZero_8(20);
    ObjectMotion_EnableActionAndSetCallback_2(8, 0x20093ac);
    BattleRuntime_WaitIfModeZero_9(80);
    ObjectMotion_SetActionVariant_1(17, 1);
    ObjectMotion_SetSpeedParameters_9(17, 0x10000, 0x8000);
    *(s32 *)(rec + 68) = 0x1999;
    *(s32 *)(rec + 72) = 0xb333;
    Audio_PlayCue_4(153);
    *(s32 *)(rec + 40) = 0x80000;
    ObjectMotion_ResetAndSetPosition_8(17, 132, 0x168);
    ObjectMotion_ResetAndSetPosition_9(18, 132, 0x168);
    BattleRuntime_WaitIfModeZero_10(40);
    ObjectMotion_SetHorizontalPositionWithTerrain_9(17, 0, 0);
    rec = Scene_GetRecord_4(8);
    *(s32 *)(rec + 24) = 0x10000;
    *(s32 *)(rec + 28) = 0x10000;
    {
        /* Set the flag word at +6. */
        s32 shown = 0x5000;

        *(u16 *)(rec + 6) = shown;
    }
    BattleRuntime_WaitIfModeZero_11(40);
    SCENE_PHASE = 0x202;
    Func_0200212a();
    ObjectMotion_SetSpeedParameters_10();
    Func_02002114(13);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
