#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02003bec();
void Func_02003bf6();
void Func_02003c10();
void Func_02003c1c();
void Func_02003c28();
void Func_02003c34();
void Func_02003c38();
void Func_02003c3c();
void Func_02003c48();
u8 *Scene_GetRecord_1();
u8 *Scene_GetRecord_2();
void Func_02003c7c();
u8 *Scene_GetRecord_3();
u8 *Scene_GetRecord_4();
void Func_02003c90();
void Func_02003cb2();
void Func_02003ce0();
void Func_02003d24();
void Func_02003d38();
void Func_02003d66();
void Func_02003d7e();
void Func_02003d96();
void Func_02003d9e();
void Func_02003dac();
void Func_02003dac_a();
void Func_02003db2();
void Func_02003dba();
void Func_02003dba_a();
void Func_02003dc8();
void Func_02003dca();
void Func_02003dcc();
s32 Func_02003dd4();
u8 *Object_SetModeById_1();
void Func_02003de0();
void Func_02003de2();
void Func_02003de8();
void Func_02003dee();
void Func_02003dee_a();
void Func_02003df0();
void Func_02003dfa();
void Func_02003e02();
void Func_02003e06();
void Func_02003e08();
void Func_02003e0c();
u8 *Scene_GetRecord_6();
void Func_02003e1a();
void Func_02003e22();
void Func_02003e26();
void Func_02003e28();
void Func_02003e28_a();
void Func_02003e28_b();
void Func_02003e2c();
void Func_02003e3c();
void Func_02003e3c_a();
void Func_02003e3e();
void Func_02003e52();
void Func_02003e58();
u8 *Scene_GetRecord_7();
void Func_02003e62();
void Func_02003e76();
u8 *Scene_GetRecord_8();
void Func_02003e84();
void Func_02003e98();
void Func_02003eaa();
void Func_02003eb0();
void Func_02003eb6();
void Func_02003ec0();
void Func_02003ec6();
void Func_02003eca();
void Func_02003ecc();
void Func_02003ed0();
void Func_02003ede();
u8 *Scene_GetRecord_9();
void Func_02003ee6();
void Func_02003eee();
u8 *Scene_GetRecord_10();
void Func_02003f02();
void Func_02003f0a();
void Func_02003f0e();
u8 *Scene_GetRecord_11();
void Func_02003f12();
void Func_02003f1a();
void Func_02003f1a_a();
void Func_02003f32();
void Func_02003f46();
void Func_02003f58();
void Func_02003f68();
void Func_02003f6a();
void Func_02003f78();
void Func_02003f7a();
void Func_02003f8a();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Battle_Reset_1(args...) Func_02003c48(args)
#define Scene_GetRecord_1(args...) Func_02003c6a(args)
#define Scene_GetRecord_2(args...) Func_02003c76(args)
#define Scene_GetRecord_3(args...) Func_02003c82(args)
#define Scene_GetRecord_4(args...) Func_02003c8e(args)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_02003d66, a0, a1, a2, a3)
#define Audio_PlayCue_1(args...) Func_02003e0c(args)
#define Audio_PlayCue_2(a0) Call1(Func_02003e22, a0)
#define Battle_WaitMode0_1(args...) Func_02003dfa(args)
#define Motion_SetSpeed_1(args...) Func_02003e06(args)
#define Battle_WaitMode0_2(args...) Func_02003ce0(args)
#define RuntimeBlock_GetOffset1e0Pointer_1(args...) Func_02003dd4(args)
#define RuntimeBlock_GetOffset1e0Pointer_2(args...) Func_02003dd4(args)
#define Motion_SetSpeedLim_1(a0, a1) Call2(Func_02003dca, a0, a1)
#define Motion_CamBounds_2(a0, a1, a2, a3) Call4(Func_02003de2, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02003dee(args)
#define BattleFx_ApplyColorToSourceBuffer_1(a0, a1) Call2(Func_02003e28, a0, a1)
#define Battle_WaitMode0_3(a0, a1) Call2(Func_02003e28_a, a0, a1)
#define Battle_WaitMode0_4(args...) Func_02003e3e(args)
#define Battle_WaitMode0_5(args...) Func_02003d24(args)
#define Battle_WaitMode0_6(a0, a1) Call2(Func_02003e3c, a0, a1)
#define Battle_WaitMode0_7(args...) Func_02003e52(args)
#define Battle_WaitMode0_8(args...) Func_02003d38(args)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_02003d9e, a0, a1, a2)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Call3(Func_02003dac, a0, a1, a2)
#define Motion_SetHPosTerrain_3(a0, a1, a2) Call3(Func_02003dba, a0, a1, a2)
#define Motion_SetHPosTerrain_4(a0, a1, a2) Call3(Func_02003dc8, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02003de0(args)
#define Object_SetModeById_2(args...) Func_02003de8(args)
#define Object_SetModeById_3(args...) Func_02003df0(args)
#define Battle_WaitMode0_9(args...) Func_02003d96(args)
#define Battle_WaitMode0_10(a0, a1) Call2(Func_02003eb0, a0, a1)
#define Battle_WaitMode0_11(args...) Func_02003ec6(args)
#define Battle_WaitMode0_12(args...) Func_02003dac_a(args)
#define Battle_WaitMode0_13(args...) Func_02003db2(args)
#define Scene_GetRecord_5(args...) Func_02003dd8(args)
#define Object_SetModeById_4(args...) Func_02003e26(args)
#define Battle_WaitMode0_14(args...) Func_02003dcc(args)
#define Motion_CallWaitAnim_1(args...) Func_02003e3c_a(args)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02003e98, a0, a1, a2)
#define Motion_SetVarCb_1(args...) Func_02003e58(args)
#define Battle_WaitMode0_15(args...) Func_02003dee_a(args)
#define Scene_GetRecord_6(args...) Func_02003e14(args)
#define Object_SetModeById_5(args...) Func_02003e62(args)
#define Battle_WaitMode0_16(args...) Func_02003e08(args)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_02003ecc, a0, a1, a2)
#define Battle_WaitMode0_17(args...) Func_02003e1a(args)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_02003ede, a0, a1, a2)
#define Battle_WaitMode0_18(args...) Func_02003e2c(args)
#define Motion_ArmCb_4(args...) Func_02003eee(args)
#define Scene_GetRecord_7(args...) Func_02003e5c(args)
#define Object_SetModeById_6(args...) Func_02003eaa(args)
#define Motion_ArmCb_5(a0, a1, a2) Call3(Func_02003f0e, a0, a1, a2)
#define Battle_WaitMode0_19(args...) Func_02003e5c_a(args)
#define Scene_GetRecord_8(args...) Func_02003e82(args)
#define Object_SetModeById_7(args...) Func_02003ed0(args)
#define Battle_WaitMode0_20(args...) Func_02003e76(args)
#define Motion_CallWaitAnim_2(args...) Func_02003ee6(args)
#define Battle_WaitMode0_21(args...) Func_02003e84(args)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02003eb6, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02003ec0, a0, a1, a2)
#define Motion_SetSpeed_4(a0, a1, a2) Call3(Func_02003eca, a0, a1, a2)
#define Object_SetModeById_8(args...) Func_02003f0a(args)
#define Object_SetModeById_9(args...) Func_02003f12(args)
#define Object_SetModeById_10(args...) Func_02003f1a(args)
#define Scene_GetRecord_9(args...) Func_02003ee0(args)
#define Motion_ResetAndSetPosition_1(args...) Func_02003f02(args)
#define Scene_GetRecord_10(args...) Func_02003ef8(args)
#define Motion_ResetAndSetPosition_2(args...) Func_02003f1a_a(args)
#define Scene_GetRecord_11(args...) Func_02003f10(args)
#define Motion_ResetAndSetPosition_3(args...) Func_02003f32(args)
#define Motion_CommitPos_1(args...) Func_02003f58(args)
#define Motion_SetHPosTerrain_5(args...) Func_02003f6a(args)
#define Motion_CommitPos_2(args...) Func_02003f68(args)
#define Motion_SetHPosTerrain_6(args...) Func_02003f7a(args)
#define Motion_CommitPos_3(args...) Func_02003f78(args)
#define Motion_SetHPosTerrain_7(args...) Func_02003f8a(args)
#define Battle_SchedShoulder_1(args...) Func_02003f46(args)
u8 *Func_02003c6a();
u8 *Func_02003c76();
u8 *Func_02003c82();
u8 *Func_02003c8e();
u8 *Func_02003dd8();
void Func_02003dd8_a();
u8 *Func_02003e14();
u8 *Func_02003e5c();
void Func_02003e5c_a();
u8 *Func_02003e82();
u8 *Func_02003ee0();
u8 *Func_02003ef8();
u8 *Func_02003f10();

/* Newly named call sites: the mechanical pass left these spelled raw because
 * their calling form (a cast, or an odd argument count) did not match its
 * patterns, or because the engine function itself had no name yet. */
#define Object_SetModeById_12(a0, a1) Call2(Func_02003dd8_a, a0, a1)

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

/* Status/phase word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))

/* Two s16 fields (offsets 0xa and 0x12) read together from an actor slot
 * record and forwarded as a pair of arguments. */
#define ACTOR_FIELD_0XA(record) (*(s16 *)((record) + 10))
#define ACTOR_FIELD_0X12(record) (*(s16 *)((record) + 18))

/* Configures four actor slots (0-3), advances the shared scene phase, then
 * drives handles 1-3 through a sequence of timed calls, some of which use
 * the two s16 fields read back from an actor slot record. */
void Scene_RunFourActorPresentation(void)
{
    u32 i;
    u8 *record;

    Battle_Reset_1();
    Func_02003c3c(); /* main:08077268 */
    record = Scene_GetRecord_1(0); /* main:0808a080 */
    Func_02003c10(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_2(1); /* main:0808a080 */
    Func_02003c1c(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_3(2); /* main:0808a080 */
    Func_02003c28(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_4(3); /* main:0808a080 */
    Func_02003c34(record, 0); /* main:080091e0 */
    Motion_CamBounds_1(0x1300000, -1, 0x780000, 0);
    Func_02003bec(1); /* main:080000c0 */
    Func_02003c38(); /* main:08009128 */
    Func_02003bf6(1); /* main:080000c0 */
    Audio_PlayCue_1(141);
    Call3(Func_02003c7c, 0x50000, 0x50000, 0x10000); /* main:080091f0 */
    Audio_PlayCue_2(0x121);
    Call3(Func_02003c90, -1, -1, 0xe666); /* main:080091f0 */
    SCENE_PHASE = 0x100;
    Battle_WaitMode0_1(); /* main:0808a360 */
    Motion_SetSpeed_1(); /* main:0808a370 */
    Func_02003cb2(); /* main:080091f8 */
    Battle_WaitMode0_2(30);
    /* Clear the byte at offset 85 of the record RuntimeBlock_GetOffset1e0Pointer_1() returns. */
    *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_2() + 85) = 0;
    Motion_SetSpeedLim_1(0xcccc, 0x1999); /* speed_limit, acceleration */
    Motion_CamBounds_2(0x2000000, -0x180000, 0xa00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleFx_ApplyColorToSourceBuffer_1(0x10000, 0);
    Battle_WaitMode0_3(0x10005, 0); /* main:0808a330 */
    Battle_WaitMode0_4(50); /* main:0808a348 */
    Battle_WaitMode0_5(50);
    Battle_WaitMode0_6(0x7fff, 0); /* main:0808a330 */
    Battle_WaitMode0_7(30); /* main:0808a348 */
    Battle_WaitMode0_8(30);
    Motion_SetHPosTerrain_1(0, 0x1f80000, 0xa80000);
    Motion_SetHPosTerrain_2(1, 0x2100000, 0x900000);
    Motion_SetHPosTerrain_3(2, 0x1e80000, 0x900000);
    Motion_SetHPosTerrain_4(3, 0x2000000, 0x980000);
    Object_SetModeById_12(0, 19); /* object 0, action 19 */
    Object_SetModeById_1(1, 19);
    Object_SetModeById_2(2, 19);
    Object_SetModeById_3(3, 19);
    Battle_WaitMode0_9(10);
    Battle_WaitMode0_10(0x10000, 0); /* main:0808a330 */
    Battle_WaitMode0_11(30); /* main:0808a348 */
    Battle_WaitMode0_12(30);
    Battle_WaitMode0_13(80);
    record = Scene_GetRecord_5(0);
    Func_02003d7e(record, 1); /* main:080091e0 */
    Object_SetModeById_4(0, 1);
    Battle_WaitMode0_14(30);
    Motion_CallWaitAnim_1(0, 4);
    Motion_ArmCb_1(0, 0xc000, 20);
    Motion_SetVarCb_1(0, 2);
    Battle_WaitMode0_15(60);
    record = Scene_GetRecord_6(1); /* main:0808a080 */
    Func_02003dba_a(record, 1); /* main:080091e0 */
    Object_SetModeById_5(1, 1);
    Battle_WaitMode0_16(20);
    Motion_ArmCb_2(1, 0x2000, 0);
    Battle_WaitMode0_17(20);
    Motion_ArmCb_3(1, 0x6000, 0);
    Battle_WaitMode0_18(20);
    Motion_ArmCb_4(1, 0, 0);
    record = Scene_GetRecord_7(2);
    Func_02003e02(record, 1); /* main:080091e0 */
    Object_SetModeById_6(2, 1);
    Motion_ArmCb_5(1, 0x6000, 0);
    Battle_WaitMode0_19(40);
    record = Scene_GetRecord_8(3); /* main:0808a080 */
    Func_02003e28_b(record, 1); /* main:080091e0 */
    Object_SetModeById_7(3, 1);
    Battle_WaitMode0_20(20);
    Motion_CallWaitAnim_2(3, 3);
    Battle_WaitMode0_21(20);
    Motion_SetSpeed_2(1, 0xcccc, 0x6666);
    Motion_SetSpeed_3(2, 0xcccc, 0x6666);
    Motion_SetSpeed_4(3, 0xcccc, 0x6666);
    Object_SetModeById_8(1, 2);
    Object_SetModeById_9(2, 2);
    Object_SetModeById_10(3, 2);
    record = Scene_GetRecord_9(0); /* main:0808a080 */
    if (record != 0) {
        Motion_ResetAndSetPosition_1(1, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = Scene_GetRecord_10(0); /* main:0808a080 */
    if (record != 0) {
        Motion_ResetAndSetPosition_2(2, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = Scene_GetRecord_11(0); /* main:0808a080 */
    if (record != 0) {
        Motion_ResetAndSetPosition_3(3, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    Motion_CommitPos_1(3);
    Motion_SetHPosTerrain_5(3, 0, 0);
    Motion_CommitPos_2(2);
    Motion_SetHPosTerrain_6(2, 0, 0);
    Motion_CommitPos_3(1);
    Motion_SetHPosTerrain_7(1, 0, 0);
    Battle_SchedShoulder_1();
}
