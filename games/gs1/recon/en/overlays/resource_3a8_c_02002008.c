#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02005b3e();
void Func_02005b52();
void Func_02005bd0();
void Func_02005c06();
void Func_02005ca2();
void Func_02005cac();
void Func_02005cbc();
u8 *Scene_GetRecord_1();
void Func_02005cca();
u8 *Scene_GetRecord_2();
s32 Func_02005cde();
s32 Func_02005cde_a();
void Func_02005cea();
s32 Func_02005cf0();
void Func_02005d0e();
u8 *Scene_GetRecord_5();
u8 *Scene_GetRecord_6();
void Func_02005d38();
void Func_02005d3a();
void Func_02005d4c();
void Func_02005d52();
void Func_02005d52_a();
void Func_02005d6c();
void Func_02005d74();
s32 Func_02005d88();
void Func_02005d98();
void Func_02005dae();
void Func_02005db0();
void Func_02005dba();
void Func_02005dc8();
void Func_02005dd0();
void Func_02005dec();
void Func_02005dec_a();
void Func_02005dee();
void Func_02005e0e();
void Func_02005e24();
void Func_02005e28();
void Func_02005e2a();
void Func_02005e5a();
void Func_02005e64();
void Func_02005e7c();
void Func_02005e80();
void Func_02005e86();
void Func_02005e8c();
void Func_02005e96();
void Func_02005ea2();
void Func_02005eba();
void Func_02005ed4();
void Func_02005ee6();
void Func_02005ee8();
void Func_02005eec();
void Func_02005ef4();
void Func_02005ef6();
void Func_02005efa();
void Func_02005efe();
void Func_02005f08();
void Func_02005f14();
void Func_02005f16();
void Func_02005f26();
void Func_02005f28();
void Func_02005f2c();
void Func_02005f3e();
void Func_02005f44();
void Func_02005f4e();
void Func_02005f56();
void Func_02005f5a();
void Func_02005f6e();
void Func_02005f7e();
void Func_02005f80();
void Func_02005f80_a();
s32 Func_02005f84();
void Func_02005f98();
void Func_02005fbe();
void Func_02005fca();
void Func_02005fd4();
void Func_02005fda();
void Func_02005ff0();
void Func_02005ff0_a();
void Func_02005ff6();
void Func_02005ffc();
void Func_0200600c();
void Func_02006010();
void Func_02006018();
void Func_0200601c();
s32 Func_02006020();
void Func_02006026();
void Func_02006030();
void Func_0200604c();
void Func_0200604e();
void Func_0200605a();
void Func_0200605a_a();
void Func_02006084();
void Func_0200608a();
void Func_020060b4();
void Func_020060c2();
void Func_020060c6();
void Func_020060d2();
void Func_020060e2();
void Func_020060e4();
void Func_020060ee();
void Func_020060fe();
void Func_02006104();
void Func_02006106();
void Func_02006118();
void Func_0200611a();
void Func_0200611c();
void Func_0200611e();
void Func_02006128();
void Func_02006148();
void Func_02006150();
u8 *Motion_SetSpeed_5();
void Func_02006156();
u8 *Motion_SetSpeed_6();
void Func_02006160();
void Func_02006166();
s32 Func_02006168();
void Func_0200616c();
void Func_02006170();
void Func_02006174();
s32 Func_0200617a();
void Func_02006180();
void Func_02006184();
void Func_02006184_a();
void Func_02006186();
void Func_0200618a();
void Func_0200618c();
void Func_02006194();
void Func_020061a0();
void Func_020061a8();
u8 *Scene_GetRecord_12();
u8 *Scene_GetRecord_13();
void Func_020061bc();
void Func_020061bc_a();
void Func_020061c4();
void Func_020061cc();
void Func_020061d6();
void Func_020061f8();
void Func_020061fe();
void Func_0200620c();
void Func_0200623a();
void Func_02006256();
void Func_02006274();
void Func_02006278();
void Func_02006290();
void Func_02006294();
void Func_02006296();
void Func_020062a0();
void Func_020062a2();
void Func_020062a4();
void Func_020062a6();
void Func_020062b6();
void Func_020062b6_a();
void Func_020062d8();
void Func_020062e2();
void Func_0200633e();
void Func_020063b8();
void Func_020063be();
void Func_020063ca();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Battle_Reset_1(args...) Func_02005c06(args)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_02005d38, a0, a1, a2, a3)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_02005cac, a0, a1, a2)
#define Motion_SetSpeedLim_1(a0, a1) Call2(Func_02005d52, a0, a1)
#define Motion_CamBounds_2(a0, a1, a2, a3) Call4(Func_02005d6c, a0, a1, a2, a3)
#define Battle_WaitMode0_1(args...) Func_02005d98(args)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02005ca2, a0, a1, a2)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_02005cde, a0, a1, a2)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02005cbc, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02005cca, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02005cc0(args)
#define Scene_GetRecord_2(args...) Func_02005ccc(args)
#define Scene_GetRecord_3(args...) Func_02005cde_a(args)
#define Scene_GetRecord_4(args...) Func_02005cf0(args)
#define Motion_ResetPosMode2_1(args...) Func_02005d3a(args)
#define Motion_SetPosReset_2(args...) Func_02005d4c(args)
#define Object_SetModeById_1(args...) Func_02005d74(args)
#define Battle_WaitMode0_2(args...) Func_02005cea(args)
#define Scene_GetRecord_5(args...) Func_02005d20(args)
#define Scene_GetRecord_6(args...) Func_02005d30(args)
#define Battle_WaitMode0_3(args...) Func_02005d0e(args)
#define Motion_CallWaitAnim_1(args...) Func_02005dae(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02005dec, a0)
#define Battle_RunThenWaitIfModeZero_1(args...) Func_02005e0e(args)
#define Motion_ArmCb_1(args...) Func_02005e28(args)
#define Motion_CallWaitAnim_2(args...) Func_02005dd0(args)
#define Battle_RunThenWaitIfModeZero_2(args...) Func_02005e2a(args)
#define Battle_WaitMode0_4(a0, a1) Call2(Func_02005e64, a0, a1)
#define Battle_WaitMode0_5(args...) Func_02005d52_a(args)
#define Scene_GetRecord_7(a0) Value1(Func_02005d88, a0)
#define Motion_SetHPosTerrain_2(args...) Func_02005dee(args)
#define Motion_SetSpeed_4(a0, a1, a2) Call3(Func_02005db0, a0, a1, a2)
#define Motion_SetPosReset_3(a0, a1, a2) Call3(Func_02005dec_a, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_02005e80, a0, a1, a2)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_02005e8c, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_3(a0, a1, a2) Call3(Func_02005e86, a0, a1, a2)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_02005ea2, a0, a1, a2)
#define Motion_SetPosReset_4(args...) Func_02005e24(args)
#define Battle_WaitMode0_6(args...) Func_02005dba(args)
#define Motion_CallWaitAnim_3(args...) Func_02005e5a(args)
#define Battle_WaitMode0_7(args...) Func_02005dc8(args)
#define Battle_RunThenWaitIfModeZero_4(args...) Func_02005eba(args)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_02005eec, a0, a1, a2)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_02005ef6, a0, a1, a2)
#define Motion_ArmCb_5(args...) Func_02005ee8(args)
#define Motion_ArmCb_6(a0, a1, a2) Call3(Func_02005ef4, a0, a1, a2)
#define Motion_ArmCb_7(args...) Func_02005efe(args)
#define Motion_ArmCb_8(args...) Func_02005f08(args)
#define BattleFx_SpawnLinked_3(a0, a1, a2) Call3(Func_02005f2c, a0, a1, a2)
#define Motion_SetVarCbObj_1(args...) Func_02005ed4(args)
#define Battle_RunThenWaitIfModeZero_5(args...) Func_02005f16(args)
#define Motion_SetVarCbObj_2(args...) Func_02005ee6(args)
#define Battle_RunThenWaitIfModeZero_6(args...) Func_02005f28(args)
#define BattleFx_SpawnLinked_4(a0, a1, a2) Call3(Func_02005f5a, a0, a1, a2)
#define Motion_SetVarCb_1(args...) Func_02005efa(args)
#define Battle_RunThenWaitIfModeZero_7(args...) Func_02005f44(args)
#define Motion_SetVarCbObj_3(args...) Func_02005f14(args)
#define Battle_RunThenWaitIfModeZero_8(args...) Func_02005f56(args)
#define Motion_SetVarCbObj_4(args...) Func_02005f26(args)
#define Battle_WaitMode0_8(args...) Func_02005e7c(args)
#define Motion_ArmCb_9(a0, a1, a2) Call3(Func_02005f80, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_1(a0, a1) Call2(Func_02005f80_a, a0, a1)
#define Battle_WaitMode0_9(args...) Func_02005e96(args)
#define Motion_SetVarCbObj_5(args...) Func_02005f4e(args)
#define Motion_CallWaitAnim_4(args...) Func_02005f3e(args)
#define Battle_RunThenWaitIfModeZero_9(args...) Func_02005f98(args)
#define BattleFx_SpawnLinked_5(a0, a1, a2) Call3(Func_02005fca, a0, a1, a2)
#define Motion_ArmCb_10(a0, a1, a2) Call3(Func_02005fbe, a0, a1, a2)
#define Motion_SetVarCbObj_6(args...) Func_02005f7e(args)
#define Motion_CallWaitAnim_5(args...) Func_02005f6e(args)
#define Motion_ArmCb_11(a0, a1, a2) Call3(Func_02005fda, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_10(args...) Func_02005fd4(args)
#define Motion_ArmCb_12(a0, a1, a2) Call3(Func_02005ff0, a0, a1, a2)
#define Motion_ArmCb_13(a0, a1, a2) Call3(Func_02005ffc, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_11(args...) Func_02005ff6(args)
#define Motion_ArmCb_14(args...) Func_02006010(args)
#define Motion_ArmCb_15(a0, a1, a2) Call3(Func_0200601c, a0, a1, a2)
#define Motion_ArmCb_16(args...) Func_02006026(args)
#define Motion_ArmCb_17(args...) Func_02006030(args)
#define Motion_SetVarCbObj_7(args...) Func_02005ff0_a(args)
#define Motion_SetSpeed_5(a0, a1) Value2(Func_02006020, a0, a1)
#define Motion_ArmCb_18(a0, a1, a2) Call3(Func_0200604c, a0, a1, a2)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02005f84, a0, a1)
#define Motion_SetVarCbObj_8(args...) Func_02006018(args)
#define Battle_RunThenWaitIfModeZero_12(a0, a1, a2) Call3(Func_0200605a, a0, a1, a2)
#define Motion_CallWaitAnim_6(args...) Func_0200605a_a(args)
#define Battle_RunThenWaitIfModeZero_13(a0, a1, a2) Call3(Func_020060b4, a0, a1, a2)
#define Motion_SetVarCbObj_9(args...) Func_02006084(args)
#define Battle_RunThenWaitIfModeZero_14(args...) Func_020060c6(args)
#define Motion_ArmCb_19(a0, a1, a2) Call3(Func_020060e2, a0, a1, a2)
#define Motion_CallWaitAnim_7(args...) Func_0200608a(args)
#define Battle_RunThenWaitIfModeZero_15(args...) Func_020060e4(args)
#define Battle_WaitMode0_10(a0, a1) Call2(Func_0200611e, a0, a1)
#define Battle_WaitMode0_11(args...) Func_0200600c(args)
#define Battle_RunThenWaitIfModeZero_16(args...) Func_020060fe(args)
#define Motion_ArmCb_20(a0, a1, a2) Call3(Func_0200611a, a0, a1, a2)
#define Motion_CallWaitAnim_8(args...) Func_020060c2(args)
#define Battle_RunThenWaitIfModeZero_17(args...) Func_0200611c(args)
#define Battle_WaitMode0_12(a0, a1) Call2(Func_02006156, a0, a1)
#define Battle_WaitMode0_13(a0, a1) Call2(Func_02006160, a0, a1)
#define Battle_WaitMode0_14(args...) Func_0200604e(args)
#define Motion_SetVarCbObj_10(args...) Func_02006106(args)
#define Battle_RunThenWaitIfModeZero_18(args...) Func_02006148(args)
#define Motion_SetVarCbObj_11(args...) Func_02006118(args)
#define Motion_ArmCb_21(a0, a1, a2) Call3(Func_0200616c, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_19(args...) Func_02006166(args)
#define Motion_ArmCb_22(args...) Func_02006180(args)
#define Motion_CallWaitAnim_9(args...) Func_02006128(args)
#define Motion_ArmCb_23(a0, a1, a2) Call3(Func_02006194, a0, a1, a2)
#define BattleEv_RunWait_1(args...) Func_02006184(args)
#define Motion_ArmCb_24(a0, a1, a2) Call3(Func_020061a8, a0, a1, a2)
#define Motion_CallWaitAnim_10(args...) Func_02006150(args)
#define Motion_ArmCb_25(a0, a1, a2) Call3(Func_020061bc, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_2(a0, a1) Call2(Func_020061bc_a, a0, a1)
#define Battle_WaitMode0_15(args...) Func_020060d2(args)
#define Motion_SetVarCbObj_12(args...) Func_0200618a(args)
#define Battle_RunThenWaitIfModeZero_20(a0, a1, a2) Call3(Func_020061cc, a0, a1, a2)
#define Motion_CallWaitAnim_11(args...) Func_02006184_a(args)
#define Motion_CallWaitAnim_12(args...) Func_0200618c(args)
#define Motion_ArmCb_26(a0, a1, a2) Call3(Func_020061f8, a0, a1, a2)
#define Motion_CallWaitAnim_13(args...) Func_020061a0(args)
#define Motion_SetSpeed_6(args...) Func_02006152_a(args)
#define Motion_SetSpeed_7(args...) Func_0200615c_a(args)
#define Scene_GetRecord_8(args...) Func_02006168(args)
#define Scene_GetRecord_9(args...) Func_0200617a(args)
#define Motion_ResetPosMode2_2(args...) Func_020061c4(args)
#define Motion_SetPosReset_5(args...) Func_020061d6(args)
#define Object_SetModeById_2(args...) Func_020061fe(args)
#define Battle_WaitMode0_16(args...) Func_02006174(args)
#define Scene_GetRecord_10(args...) Func_020061aa(args)
#define Scene_GetRecord_11(args...) Func_020061ba(args)
#define Motion_SetPosReset_6(args...) Func_0200620c(args)
#define Motion_ArmCb_27(a0, a1, a2) Call3(Func_020062a0, a0, a1, a2)
#define Audio_PlayCue_1(args...) Func_0200633e(args)
#define Motion_SetPosReset_7(args...) Func_02006256(args)
#define Motion_SetHPosTerrain_3(args...) Func_02006278(args)
#define Motion_SetSpeed_8(a0, a1, a2) Call3(Func_0200623a, a0, a1, a2)
#define Motion_SetPosReset_8(args...) Func_02006274(args)
#define Motion_SetHPosTerrain_4(args...) Func_02006296(args)
#define Object_SetModeById_3(args...) Func_020062a6(args)
#define Motion_CallWaitAnim_14(args...) Func_020062b6(args)
#define Motion_ResetPosMode2_3(args...) Func_02006290(args)
#define Motion_SetPosReset_9(args...) Func_020062a2(args)
#define Motion_ResetPosMode2_4(args...) Func_020062a4(args)
#define Motion_SetPosReset_10(args...) Func_020062b6_a(args)
#define Motion_SetHPosTerrain_5(args...) Func_020062d8(args)
#define Motion_SetHPosTerrain_6(args...) Func_020062e2(args)
#define Motion_SetSpeed_9(args...) Func_020063ca(args)
#define Battle_SchedShoulder_1(args...) Func_02006294(args)
u8 *Func_02005cc0();
u8 *Func_02005ccc();
u8 *Func_02005d20();
u8 *Func_02005d30();
u8 *Func_02006152();
u8 *Func_0200615c();
u8 *Func_02006152_a();
u8 *Func_0200615c_a();
u8 *Func_020061aa();
u8 *Func_020061ba();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Field at 0x1c8 of the shared scene work record. */
#define SCENE_FIELD_1C8 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c8))

/* Field at 0x1c0 of the shared scene work record. */
#define SCENE_FIELD_1C0 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c0))

/* Runs a scripted sequence for two actors (8 and 9): sets up their sprite
 * records, moves and animates them in lockstep through a series of timed
 * steps, then hands off to a third actor (2) and a couple of standalone
 * calls (5) before advancing the shared scene step counter and phase word. */
void Scene_RunTwoActorCutsceneSequence(void)
{
    u32 i;
    u8 *record;

    Battle_Reset_1();
    Motion_CamBounds_1(-1, -1, -1, 0);
    Func_02005b3e(1);
    Motion_SetHPosTerrain_1(0, 0xc00000, 0x1560000);
    Func_02005b52(1);
    Motion_SetSpeedLim_1(0x3333, 0x666);
    Motion_CamBounds_2(0xc00000, -1, 0xfc0000, 1);
    SCENE_FIELD_1C8 = 40;
    Battle_WaitMode0_1();
    Motion_SetSpeed_1(0, 0xcccc, 0x6666);
    Motion_SetPosReset_1(0, 192, 0x116);
    Motion_SetSpeed_2(8, 0x10000, 0x8000);
    Motion_SetSpeed_3(9, 0x10000, 0x8000);
    record = Scene_GetRecord_1(8);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    record = Scene_GetRecord_2(9);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0x5000;

        *(u16 *)(record + 6) = shown;
    }
    Func_02005bd0(1);
    /* Clear bit 0 of the flag byte at +90. */
    *(u8 *)(Scene_GetRecord_3(8) + 90) &= 254;
    *(u8 *)(Scene_GetRecord_4(9) + 90) &= 254;
    Motion_ResetPosMode2_1(8, 184, 232);
    Motion_SetPosReset_2(9, 198, 232);
    Object_SetModeById_1(8, 1);
    Battle_WaitMode0_2(20);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 *record = Scene_GetRecord_5(8);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    {
        u8 *record = Scene_GetRecord_6(9);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    Battle_WaitMode0_3(20);
    Motion_CallWaitAnim_1(8, 4);
    SceneWork_SetStepValue_1(0x1b05);
    Battle_RunThenWaitIfModeZero_1(8, 0, 10);
    Motion_ArmCb_1(9, 0x5000, 10);
    Motion_CallWaitAnim_2(9, 3);
    Battle_RunThenWaitIfModeZero_2(9, 0, 10);
    Battle_WaitMode0_4(0, 0x102);
    Battle_WaitMode0_5(60);
    record = Scene_GetRecord_7(0);
    if (record != 0) {
        Motion_SetHPosTerrain_2(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Motion_SetSpeed_4(2, 0xcccc, 0x6666);
    Motion_SetPosReset_3(2, 212, 0x10c);
    Motion_ArmCb_2(0, 0xe000, 0);
    Motion_ArmCb_3(2, 0x6000, 20);
    Battle_RunThenWaitIfModeZero_3(0x4002, 0, 20);
    Motion_ArmCb_4(0, 0xc000, 0);
    Motion_SetPosReset_4(2, 202, 254);
    Battle_WaitMode0_6(20);
    Motion_CallWaitAnim_3(2, 3);
    Battle_WaitMode0_7(10);
    Battle_RunThenWaitIfModeZero_4(2, 0, 10);
    BattleFx_SpawnLinked_1(8, 0x101, 0);
    BattleFx_SpawnLinked_2(9, 0x101, 40);
    Motion_ArmCb_5(8, 0, 0);
    Motion_ArmCb_6(9, 0x8000, 40);
    Motion_ArmCb_7(8, 0x3000, 0);
    Motion_ArmCb_8(9, 0x3000, 20);
    BattleFx_SpawnLinked_3(2, 0x102, 60);
    Motion_SetVarCbObj_1(2, 1);
    Battle_RunThenWaitIfModeZero_5(2, 0, 10);
    Motion_SetVarCbObj_2(8, 2);
    Battle_RunThenWaitIfModeZero_6(8, 0, 10);
    BattleFx_SpawnLinked_4(8, 0x105, 60);
    Motion_SetVarCb_1(8, 2);
    Battle_RunThenWaitIfModeZero_7(8, 0, 10);
    Motion_SetVarCbObj_3(9, 2);
    Battle_RunThenWaitIfModeZero_8(9, 0, 20);
    Motion_SetVarCbObj_4(2, 1);
    Battle_WaitMode0_8(10);
    Motion_ArmCb_9(2, 0x6000, 10);
    BattleEventRuntime_ProcessAction_1(0x4002, 0);
    Battle_WaitMode0_9(10);
    Motion_SetVarCbObj_5(8, 1);
    Motion_CallWaitAnim_4(8, 4);
    Battle_RunThenWaitIfModeZero_9(8, 0, 10);
    BattleFx_SpawnLinked_5(2, 0x101, 40);
    Motion_ArmCb_10(2, 0xa000, 20);
    Motion_SetVarCbObj_6(9, 1);
    Motion_CallWaitAnim_5(9, 3);
    Motion_ArmCb_11(2, 0xc000, 0);
    Battle_RunThenWaitIfModeZero_10(9, 0, 10);
    Motion_ArmCb_12(2, 0x6000, 60);
    Motion_ArmCb_13(2, 0xc000, 10);
    Battle_RunThenWaitIfModeZero_11(2, 0, 10);
    Motion_ArmCb_14(8, 0, 0);
    Motion_ArmCb_15(9, 0x8000, 40);
    Motion_ArmCb_16(8, 0x3000, 0);
    Motion_ArmCb_17(9, 0x3000, 10);
    Motion_SetVarCbObj_7(8, 1);
    Motion_SetSpeed_5(8, 0);
    Motion_ArmCb_18(2, 0x6000, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Motion_SetVarCbObj_8(2, 2);
        Battle_RunThenWaitIfModeZero_12(0x4002, 0, 10);
        bump_step(1);
    } else {
        bump_step(1);
        Motion_CallWaitAnim_6(2, 4);
        Battle_RunThenWaitIfModeZero_13(0x4002, 0, 10);
    }
    Motion_SetVarCbObj_9(9, 2);
    Battle_RunThenWaitIfModeZero_14(9, 0, 10);
    Motion_ArmCb_19(2, 0xc000, 10);
    Motion_CallWaitAnim_7(2, 4);
    Battle_RunThenWaitIfModeZero_15(2, 0, 10);
    Battle_WaitMode0_10(8, 0x102);
    Battle_WaitMode0_11(60);
    Battle_RunThenWaitIfModeZero_16(8, 0, 10);
    Motion_ArmCb_20(2, 0xa000, 10);
    Motion_CallWaitAnim_8(2, 3);
    Battle_RunThenWaitIfModeZero_17(2, 0, 10);
    Battle_WaitMode0_12(8, 0x102);
    Battle_WaitMode0_13(9, 0x102);
    Battle_WaitMode0_14(60);
    Motion_SetVarCbObj_10(8, 2);
    Battle_RunThenWaitIfModeZero_18(8, 0, 10);
    Motion_SetVarCbObj_11(9, 2);
    Motion_ArmCb_21(9, 0x8000, 10);
    Battle_RunThenWaitIfModeZero_19(9, 0, 10);
    Motion_ArmCb_22(8, 0, 10);
    Motion_CallWaitAnim_9(8, 3);
    Motion_ArmCb_23(8, 0x3000, 10);
    BattleEv_RunWait_1(8, 0);
    Motion_ArmCb_24(9, 0x3000, 10);
    Motion_CallWaitAnim_10(2, 3);
    Motion_ArmCb_25(2, 0x6000, 10);
    BattleEventRuntime_ProcessAction_2(0x4002, 0);
    Battle_WaitMode0_15(10);
    Motion_SetVarCbObj_12(2, 1);
    Battle_RunThenWaitIfModeZero_20(0x4002, 0, 10);
    Motion_CallWaitAnim_11(0, 3);
    Motion_CallWaitAnim_12(2, 3);
    Motion_ArmCb_26(2, 0xc000, 10);
    Motion_CallWaitAnim_13(2, 3);
    Call3((void (*)())Func_02006152, 8, 0xcccc, 0x6666);
    Call3((void (*)())Func_0200615c, 9, 0xcccc, 0x6666);
    record = Motion_SetSpeed_6(8);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Motion_SetSpeed_7(9);
    {
        s32 shown = 0x8000;

        *(u16 *)(record + 6) = shown;
    }
    /* Clear bit 0 of the flag byte at +90. */
    *(u8 *)(Scene_GetRecord_8(8) + 90) &= 254;
    *(u8 *)(Scene_GetRecord_9(9) + 90) &= 254;
    Motion_ResetPosMode2_2(8, 168, 232);
    Motion_SetPosReset_5(9, 212, 232);
    Object_SetModeById_2(8, 1);
    Battle_WaitMode0_16(20);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 *record = Scene_GetRecord_10(8);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    {
        u8 *record = Scene_GetRecord_11(9);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    Motion_SetPosReset_6(2, 192, 232);
    Motion_ArmCb_27(2, 0xc000, 0);
    Audio_PlayCue_1(188);
    Call6(Func_02006170, 36, 23, 43, 12, 2, 2);
    Func_020060ee(5);
    Call6(Func_02006186, 39, 23, 43, 12, 2, 2);
    Func_02006104(5);
    Motion_SetPosReset_7(2, 192, 222);
    Motion_SetHPosTerrain_3(2, 0, 0);
    Motion_SetSpeed_8(0, 0x13333, 0x9999);
    Motion_SetPosReset_8(0, 192, 222);
    Motion_SetHPosTerrain_4(0, 0, 0);
    Object_SetModeById_3(8, 3);
    Motion_CallWaitAnim_14(9, 3);
    Motion_ResetPosMode2_3(8, 184, 232);
    Motion_SetPosReset_9(9, 198, 232);
    Motion_ResetPosMode2_4(8, 188, 212);
    Motion_SetPosReset_10(9, 194, 212);
    Motion_SetHPosTerrain_5(8, 0, 0);
    Motion_SetHPosTerrain_6(9, 0, 0);
    SCENE_FIELD_1C8 = 24;
    SCENE_FIELD_1C0 = 0x201;
    Func_020063be();
    Motion_SetSpeed_9();
    Func_020063b8(5);
    Battle_SchedShoulder_1();
}
