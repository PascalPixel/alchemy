#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02001b36();
void Func_020051f2();
void Func_02005222();
void Func_02005228();
void Func_02005232();
void Func_0200525c();
void Func_0200525e();
void Func_0200526c();
void Func_02005274();
void Func_02005290();
void Func_020052aa();
void Func_020052b6();
void Func_020052c6();
void Func_020052d0();
void Func_020052ec();
void Func_020052fe();
void Func_02005346();
void Func_02005890();
s32 Func_020058ce();
void Func_020058da();
void Func_020058e4();
void Func_020058e4_a();
void Func_020058ee();
void Func_02005912();
void Func_02005928();
void Func_02005932();
void Func_02005950();
void Func_0200595a();
void Func_0200595e();
void Func_02005964();
void Func_02005992();
void Func_020059a0();
void Func_020059b6();
void Func_020059de();
void Func_020059e6();
void Func_020059f4();
void Func_020059f8();
void Func_020059fa();
void Func_02005a0a();
void Func_02005a0c();
void Func_02005a3c();
void Func_02005a54();
void Func_02005a58();
void Func_02005a6c();
void Func_02005a70();
void Func_02005a74();
void Func_02005a7c();
void Func_02005a7e();
void Func_02005a94();
void Func_02005aa0();
s32 Func_02005abe();
u8 *Motion_ArmCb_5();
void Func_02005af6();
void Func_02005b0a();
void Func_02005b12();
void Func_02005b14();
void Func_02005b16();
u8 *Scene_GetRecord_2();
void Func_02005b1e();
void Func_02005b34();
void Func_02005b42();
void Func_02005b42_a();
void Func_02005b48();
void Func_02005b48_a();
void Func_02005b4e();
void Func_02005b4e_a();
void Func_02005b54();
void Func_02005b5e();
void Func_02005b6e();
void Func_02005b7c();
void Func_02005b8e();
void Func_02005bb6();
void Func_02005c58();
void Func_02005cca();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Battle_Reset_1(args...) Func_02005890(args)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_020058da, a0, a1, a2)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_020058e4, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_020058ee, a0, a1, a2)
#define Audio_PlayCue_1(args...) Func_02005a94(args)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_02005932, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_020059b6, a0, a1, a2)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_02005964, a0, a1, a2)
#define Motion_SetPosReset_2(a0, a1, a2) Call3(Func_0200595a, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_020059de, a0, a1, a2)
#define GameFlag_IsSet_1(a0) Value1(Func_020058ce, a0)
#define GameFlag_Set_1(a0) Call1(Func_020058e4_a, a0)
#define Battle_WaitMode0_1(args...) Func_02005912(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020059f4, a0)
#define Audio_PlayCue_2(args...) Func_02005b0a(args)
#define Battle_WaitMode0_2(args...) Func_02005928(args)
#define Motion_SetVarCb_1(args...) Func_020059fa(args)
#define Motion_SetVarCbObj_1(args...) Func_02005a0a(args)
#define Battle_WaitMode0_3(args...) Func_02005950(args)
#define Battle_WaitMode0_4(args...) Func_0200595e(args)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_02005a70, a0, a1, a2)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_02005a7c, a0, a1, a2)
#define Battle_WaitMode0_5(args...) Func_02005992(args)
#define Battle_WaitMode0_6(args...) Func_020059a0(args)
#define Motion_SetVarCbObj_2(args...) Func_02005a6c(args)
#define Object_SetModeById_1(args...) Func_02005a54(args)
#define Motion_SetVarCbObj_3(args...) Func_02005aa0(args)
#define Battle_WaitMode0_7(args...) Func_020059e6(args)
#define Battle_WaitMode0_8(args...) Func_020059f8(args)
#define Motion_SetPosReset_3(a0, a1, a2) Call3(Func_02005a7e, a0, a1, a2)
#define Battle_WaitMode0_9(args...) Func_02005a0c(args)
#define Motion_ArmCb_5(a0, a1, a2) Call3(Func_02005b12, a0, a1, a2)
#define Motion_ArmCb_6(a0, a1, a2) Call3(Func_02005b1e, a0, a1, a2)
#define Motion_SetVarCbObj_4(args...) Func_02005af6(args)
#define Battle_WaitMode0_10(args...) Func_02005a3c(args)
#define Motion_ArmCb_7(a0, a1, a2) Call3(Func_02005b42, a0, a1, a2)
#define Battle_WaitMode0_11(args...) Func_02005a58(args)
#define BattleEv_RunWait_1(args...) Func_02005b48(args)
#define Motion_SetSpeed_4(args...) Func_02005b48_a(args)
#define Audio_PlayCue_3(args...) Func_02005c58(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_02005b4e, a0)
#define Motion_SetSpeed_5(args...) Func_02005b5e(args)
#define UiWork_WaitThenFinalizeCapacity_1(args...) Func_02005abe(args)
#define GameFlag_Set_2(a0) Call1(Func_02005a74, a0)
#define Object_SetModeById_2(args...) Func_02005b34(args)
#define Scene_GetRecord_1(args...) Func_02005ae2(args)
#define Motion_ResetAndSetPosition_1(args...) Func_02005b14(args)
#define Motion_CommitPos_1(args...) Func_02005b42_a(args)
#define Motion_SetHPosTerrain_2(args...) Func_02005b54(args)
#define BattleEv_RunWait_2(args...) Func_02005bb6(args)
#define Object_SetModeById_3(args...) Func_02005b6e(args)
#define Scene_GetRecord_2(args...) Func_02005b1c(args)
#define Motion_ResetAndSetPosition_2(args...) Func_02005b4e_a(args)
#define Motion_CommitPos_2(args...) Func_02005b7c(args)
#define Motion_SetHPosTerrain_3(args...) Func_02005b8e(args)
#define Audio_PlayCueForPartyMember_1(args...) Func_02005cca(args)
#define Battle_SchedShoulder_1(args...) Func_02005b16(args)
u8 *Func_02005ae2();
u8 *Func_02005ae2_a();
u8 *Func_02005b1c();

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

/* Configures actors 0, 1, 2 (position, movement, and animation timing), then
 * branches on whether actor 0 is already set up: one path sets up actors 0-2
 * with poses and movement, the other only advances actor 2's animation. Both
 * paths converge to check actor 0's record and, depending on that check,
 * configure either actor 2 or actor 1 from it before the scene finishes. */
void Scene_RunSetupSequence(void)
{
    u8 *record;

    Battle_Reset_1();
    Motion_SetSpeed_1(0, 0xcccc, 0x6666);
    Motion_SetSpeed_2(1, 0xcccc, 0x6666);
    Motion_SetSpeed_3(2, 0xcccc, 0x6666);
    Audio_PlayCue_1(19);
    Motion_SetPosReset_1(0, 0x180, 0x198);
    Motion_ArmCb_1(0, 0xc000, 0);
    Motion_SetHPosTerrain_1(1, 0x1800000, 0x1980000);
    Motion_SetPosReset_2(1, 0x170, 0x198);
    Motion_ArmCb_2(1, 0xd000, 20);
    if (GameFlag_IsSet_1(0x850) != 0) {
    } else {
        GameFlag_Set_1(0x850);
        Func_02005228(2, 0);
        Battle_WaitMode0_1(40);
        Func_0200525e();
        SceneWork_SetStepValue_1(0x1256);
        Audio_PlayCue_2(60);
        Battle_WaitMode0_2(30);
        Func_02005232(2, 3, 30);
        Func_020051f2(2, 30);
        Motion_SetVarCb_1(0, 1);
        Motion_SetVarCbObj_1(1, 1);
        Battle_WaitMode0_3(20);
        Func_02005274(2, 0);
        Battle_WaitMode0_4(40);
        Func_020052aa();
        Func_02005222(2, 30);
        Func_0200525c(0, 1, 50);
        Motion_ArmCb_3(0, 0xc000, 0);
        Motion_ArmCb_4(1, 0xd000, 0);
        Battle_WaitMode0_5(20);
        Func_020052b6(2, 0);
        Battle_WaitMode0_6(40);
        Func_020052ec();
        Motion_SetVarCbObj_2(2, 1);
        Func_0200526c(2, 50);
        Object_SetModeById_1(0, 3);
        Func_020052c6(1, 3, 20);
        Func_020052d0(2, 3, 20);
        Func_02005290(2, 40);
        Motion_SetVarCbObj_3(2, 1);
        Battle_WaitMode0_7(30);
        Call3((void (*)())Func_02005ae2_a, 2, 0xc000, 0);
        Battle_WaitMode0_8(30);
        Motion_SetPosReset_3(2, 0x178, 0x178);
        Battle_WaitMode0_9(40);
        Func_020052fe(0, 1, 50);
        Motion_ArmCb_5(0, 0xc000, 0);
        Motion_ArmCb_6(1, 0xd000, 0);
        Motion_SetVarCbObj_4(2, 1);
        Battle_WaitMode0_10(50);
        Func_02005346(2, 3, 30);
        Motion_ArmCb_7(2, 0x4000, 0);
        Battle_WaitMode0_11(10);
        BattleEv_RunWait_1(2, 0);
        Motion_SetSpeed_4(2, 0);
        goto L_join_setup_paths;
    }
    Audio_PlayCue_3(60);
    SceneWork_SetStepValue_2(0x125d);
    Motion_SetSpeed_5(2, 0);
    L_join_setup_paths:;
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Func_02001b36();
        GameFlag_Set_2(0x856);
        Object_SetModeById_2(2, 2);
        record = Scene_GetRecord_1(0);
        if (record != 0) {
            /* Read the two s16 fields at +10 and +18 of the record. */
            Motion_ResetAndSetPosition_1(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Motion_CommitPos_1(2);
        Motion_SetHPosTerrain_2(2, 0, 0);
    } else {
        BattleEv_RunWait_2(2, 0);
    }
    Object_SetModeById_3(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Read the two s16 fields at +10 and +18 of the record. */
        Motion_ResetAndSetPosition_2(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_2(1);
    Motion_SetHPosTerrain_3(1, 0, 0);
    Audio_PlayCueForPartyMember_1();
    Battle_SchedShoulder_1();
}
