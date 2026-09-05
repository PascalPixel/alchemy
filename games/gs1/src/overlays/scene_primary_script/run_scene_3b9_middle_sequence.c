#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020034f2();
void Func_02004280();
void Func_020042ae();
void Func_020042be();
void Func_02004308();
s32 Func_02004310();
void Func_02004318();
s32 Func_0200431c();
void Func_02004320();
s32 Func_02004326();
void Func_02004328();
void Func_02004330();
void Func_02004338();
void Func_02004340();
void Func_02004342();
void Func_02004350();
void Func_02004352();
void Func_0200435a();
void Func_0200436c();
void Func_0200437a();
void Func_02004380();
void Func_02004382();
void Func_02004388();
void Func_02004396();
void Func_020043c6();
void Func_020043d4();
void Func_020043d8();
void Func_020043da();
void Func_020043e2();
void Func_020043e4();
void Func_020043f0();
void Func_020043f2();
void Func_020043fe();
void Func_02004400();
void Func_02004402();
void Func_0200440e();
void Func_02004426();
void Func_02004428();
void Func_0200443e();
void Func_0200444a();
void Func_0200445a();
void Func_02004460();
void Func_02004468();
void Func_0200446c();
void Func_0200447a();
void Func_0200447c();
void Func_0200448a();
s32 Func_020044b0();
void Func_020044d4();
void Func_020044da();
void Func_020044e8();
void Func_020044ea();
void Func_020044f4();
void Func_020044f6();
void Func_02004500();
void Func_02004502();
void Func_0200450a();
s32 Func_02004512();
void Func_0200451c();
void Func_02004520();
void Func_02004526();
void Func_02004530();
void Func_02004536();
void Func_0200453a();
void Func_0200453c();
void Func_0200453e();
void Func_02004548();
void Func_0200455c();
void Func_0200456a();
void Func_0200456e();
void Func_0200457c();
void Func_0200458a();
void Func_02004594();
void Func_02004598();
void Func_020045ae();
void Func_020045be();
void Func_020045d6();
void Func_020045d8();
void Func_020045e6();
void Func_020045f4();
void Func_02004602();
void Func_02004630();
void Func_02004658();
void Func_0200466e();
void Func_0200469c();
void Func_020046d2();
void Func_020046ea();
void Func_020046f6();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02004280(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02004382, a0, a1, a2, a3)
#define Audio_PlayCue_1(args...) Func_020043d8(args)
#define Object_SetModeById_1(args...) Func_02004308(args)
#define Object_SetModeById_2(args...) Func_02004318(args)
#define Object_SetModeById_3(args...) Func_02004320(args)
#define Object_SetModeById_4(args...) Func_02004328(args)
#define Object_SetModeById_5(args...) Func_02004330(args)
#define Object_SetModeById_6(args...) Func_02004338(args)
#define Object_SetModeById_7(args...) Func_02004340(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02004342(args)
#define Object_SetModeById_8(args...) Func_02004352(args)
#define Object_SetModeById_9(args...) Func_0200435a(args)
#define Scene_GetRecord_1(args...) Func_02004310(args)
#define Scene_GetRecord_2(args...) Func_0200431c(args)
#define Scene_GetRecord_3(a0) Value1(Func_02004326, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02004380(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02004468(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_0200447c(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02004342(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_020043da(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02004350(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02004382, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020043c6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_0200436c(args)
#define Object_SetModeById_10(args...) Func_020043e4(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_0200437a(args)
#define Object_SetModeById_11(args...) Func_020043f2(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02004388(args)
#define Object_SetModeById_12(args...) Func_02004400(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02004396(args)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02004402, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_0200440e, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_0200448a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_02004426, a0, a1, a2)
#define Object_SetModeById_13(args...) Func_0200443e(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_020043d4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_0200446c(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_020043e2(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_0200447a(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_020043f0(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Call2(Func_02004428, a0, a1)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_020043fe(args)
#define BattleRuntime_WaitIfModeZero_12(a0, a1) Call2(Func_020044e8, a0, a1)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_0200440e(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020044ea, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_020044f6, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02004502, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_14(a0, a1) Call2(Func_0200451c, a0, a1)
#define BattleRuntime_WaitIfModeZero_15(a0, a1) Call2(Func_02004526, a0, a1)
#define BattleRuntime_WaitIfModeZero_16(a0, a1) Call2(Func_02004530, a0, a1)
#define BattleRuntime_WaitIfModeZero_17(a0, a1) Call2(Func_0200453a, a0, a1)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_02004460(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_0200453c, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02004548, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_19(a0, a1) Call2(Func_0200456a, a0, a1)
#define Scene_GetRecord_4(a0) Value1(Func_020044b0, a0)
#define Object_SetModeById_14(args...) Func_02004536(args)
#define Object_SetModeById_15(args...) Func_0200453e(args)
#define BattleRuntime_WaitIfModeZero_20(args...) Func_020044d4(args)
#define BattleRuntime_WaitIfModeZero_21(a0, a1) Call2(Func_020045be, a0, a1)
#define ObjectMotion_EnableActionAndResetMotion_1(args...) Func_0200451c(args)
#define Object_SetModeById_16(args...) Func_0200455c(args)
#define Scene_GetRecord_5(args...) Func_02004512(args)
#define BattleRuntime_WaitIfModeZero_22(args...) Func_0200450a(args)
#define Audio_PlayCue_2(args...) Func_02004658(args)
#define BattleRuntime_WaitIfModeZero_23(args...) Func_02004520(args)
#define Audio_PlayCue_3(a0) Call1(Func_0200466e, a0)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_0200456a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_5(a0, a1, a2) Call3(Func_020045ae, a0, a1, a2)
#define Audio_PlayCue_4(args...) Func_0200469c(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02004630, a0, a1, a2)
#define Object_SetModeById_17(args...) Func_020045d8(args)
#define BattleRuntime_WaitIfModeZero_24(args...) Func_0200456e(args)
#define Object_SetModeById_18(args...) Func_020045e6(args)
#define BattleRuntime_WaitIfModeZero_25(args...) Func_0200457c(args)
#define Object_SetModeById_19(args...) Func_020045f4(args)
#define BattleRuntime_WaitIfModeZero_26(args...) Func_0200458a(args)
#define Object_SetModeById_20(args...) Func_02004602(args)
#define BattleRuntime_WaitIfModeZero_27(args...) Func_02004598(args)
#define ObjectMotion_SetSpeedParameters_4(args...) Func_020046f6(args)
#define GameFlag_Set_1(a0) Call1(Func_02004594, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_020045d6(args)

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Sets up actors 8-20 (position, pose, or movement/sprite flags), advances
 * two actor records' +24 fields, advances the shared scene phase, then runs
 * a long chain of per-actor moves, pose changes, and waits. */
void FieldScene_RunMiddleSequence(void)
{
    u32 counter;
    u8 *rec;
    s32 rec2;
    u8 *work;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(-1, -1, -1, 0);
    Audio_PlayCue_1(247);
    Object_SetModeById_1(8, 2);
    Call2((void (*)())Func_02004310, 9, 2);
    Object_SetModeById_2(10, 2);
    Object_SetModeById_3(11, 2);
    Object_SetModeById_4(12, 2);
    Object_SetModeById_5(13, 2);
    Object_SetModeById_6(14, 0);
    Object_SetModeById_7(15, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(16, 0, 0);
    Object_SetModeById_8(17, 0);
    Object_SetModeById_9(18, 0);
    rec2 = Scene_GetRecord_1(21);
    Func_020042ae(rec2, 0);
    rec2 = Scene_GetRecord_2(19);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    rec2 = Scene_GetRecord_3(20);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    Func_020042ae(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(0, 0, 0);
    Func_020042be(1);
    /* Advance the scene phase/status word at +0x1c0 and its companion word
     * at +0x1c8 of the shared scene work record. */
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c0)) = 0x200;
    *(volatile s32 *)((work + 0x1c8)) = 32;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1();
    BattleRuntime_WaitIfModeZero_2(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(17, 1);
    BattleRuntime_WaitIfModeZero_3(20);
    ObjectMotion_SetSpeedParameters_2(17, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1(17, 164, 0x388);
    BattleRuntime_WaitIfModeZero_4(20);
    Object_SetModeById_10(17, 9);
    BattleRuntime_WaitIfModeZero_5(40);
    Object_SetModeById_11(17, 10);
    BattleRuntime_WaitIfModeZero_6(60);
    Object_SetModeById_12(17, 1);
    BattleRuntime_WaitIfModeZero_7(20);
    ObjectMotion_SetPositionAndReset_2(17, 164, 0x398);
    ObjectMotion_SetPositionAndReset_3(17, 185, 0x398);
    ObjectMotion_ArmCallback_1(17, 0xc000, 20);
    ObjectMotion_SetPositionAndReset_4(17, 185, 0x394);
    Object_SetModeById_13(17, 11);
    BattleRuntime_WaitIfModeZero_8(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(17, 1);
    BattleRuntime_WaitIfModeZero_9(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(17, 3);
    BattleRuntime_WaitIfModeZero_10(40);
    ObjectMotion_EnableActionAndSetCallback_1(17, 0x200af88);
    BattleRuntime_WaitIfModeZero_11(80);
    BattleRuntime_WaitIfModeZero_12(17, 0x102);
    BattleRuntime_WaitIfModeZero_13(60);
    ObjectMotion_ArmCallback_2(14, 0xd000, 0);
    ObjectMotion_ArmCallback_3(15, 0x5000, 0);
    ObjectMotion_ArmCallback_4(18, 0x8000, 20);
    BattleRuntime_WaitIfModeZero_14(14, 0x102);
    BattleRuntime_WaitIfModeZero_15(15, 0x102);
    BattleRuntime_WaitIfModeZero_16(17, 0x102);
    BattleRuntime_WaitIfModeZero_17(18, 0x102);
    BattleRuntime_WaitIfModeZero_18(60);
    ObjectMotion_ArmCallback_5(14, 0x3000, 0);
    ObjectMotion_ArmCallback_6(15, 0x3000, 0);
    Value2(Func_020034f2, 18, 0x3000);
    BattleRuntime_WaitIfModeZero_19(17, 0x101);
    /* Clear the flag byte at +85, then step the +12 field back and forth
     * 20 times, waiting between each step. */
    rec = Scene_GetRecord_4(21);
    rec[85] = 0;
    for (counter = 0; counter < 20; counter++) {
        *(volatile s32 *)(rec + 12) += 0x9999;
        Func_0200444a(4);
        *(volatile s32 *)(rec + 12) += -0x4ccc;
        Func_0200445a(4);
    }
    Object_SetModeById_14(19, 6);
    Object_SetModeById_15(20, 6);
    BattleRuntime_WaitIfModeZero_20(60);
    BattleRuntime_WaitIfModeZero_21(17, 0x100);
    ObjectMotion_EnableActionAndResetMotion_1(17);
    Object_SetModeById_16(17, 1);
    rec2 = Scene_GetRecord_5(17);
    {
        /* Set the u16 field at +6. */
        s32 shown = 0xd000;

        *(volatile u16 *)(rec2 + 6) = shown;
    }
    rec[85] = 3;
    *(volatile s32 *)(rec + 24) = 0x10000;
    *(volatile s32 *)(rec + 28) = 0x10000;
    BattleRuntime_WaitIfModeZero_22(10);
    Audio_PlayCue_2(107);
    Func_020044da(0x10000, 0x10000, 0x10000);
    BattleRuntime_WaitIfModeZero_23(10);
    Audio_PlayCue_3(0x121);
    Call3(Func_020044f4, -1, -1, 0xe666);
    Func_02004500();
    ObjectMotion_SetSpeedParameters_3(17, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndReset_5(17, 208, 0x3a0);
    Audio_PlayCue_4(92);
    ObjectMotion_ArmCallback_7(17, 0x3000, 20);
    Object_SetModeById_17(17, 9);
    BattleRuntime_WaitIfModeZero_24(20);
    Object_SetModeById_18(17, 10);
    BattleRuntime_WaitIfModeZero_25(40);
    Object_SetModeById_19(17, 9);
    BattleRuntime_WaitIfModeZero_26(20);
    Object_SetModeById_20(17, 10);
    BattleRuntime_WaitIfModeZero_27(80);
    /* Advance the scene phase/status word at +0x1c0 and its companion word
     * at +0x1c8 of the shared scene work record. */
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c0)) = 0x201;
    *(volatile s32 *)((work + 0x1c8)) = 16;
    Func_020046ea();
    ObjectMotion_SetSpeedParameters_4();
    GameFlag_Set_1(0x8a4);
    Func_020046d2(69);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
