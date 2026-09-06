#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02007490();
void Func_020074b6();
void Func_020074d6();
void Func_020075d0();
s32 Func_02007622();
void Func_02007678();
void Func_0200767a();
void Func_0200767c();
void Func_0200769c();
void Func_020076a0();
void Func_020076aa();
void Func_020076ae();
void Func_020076b2();
void Func_020076ba();
void Func_020076c8();
void Func_020076d0();
void Func_020076e6();
void Func_020076fa();
void Func_020076fc();
void Func_02007730();
void Func_02007734();
void Func_020077a0();
void Func_020077c6();
void Func_020077f0();
void Func_0200781e();
void Func_0200782a();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_020075d0(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_020076fa, a0, a1, a2, a3)
#define Object_SetModeById_1(args...) Func_02007678(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_0200767c, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02007622(args)
#define RatioHistory_RecordAndScheduleCallback_1(a0, a1) Call2(Func_020077c6, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_020076a0(args)
#define ObjectMotion_SetPositionAndReset_1(args...) Func_02007730(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020077a0(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_0200767a, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_1(a0, a1, a2) Call3(Func_020076ae, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_2(a0, a1, a2) Call3(Func_020076ba, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_0200769c, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_3(a0, a1, a2) Call3(Func_020076d0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_020076b2, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_4(a0, a1, a2) Call3(Func_020076e6, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_020076c8, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_5(a0, a1, a2) Call3(Func_020076fc, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_02007734(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_020076aa(args)
#define ObjectMotion_SetSpeedParameters_5(args...) Func_0200782a(args)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))

/* The actor index every call in this scene step operates on. */
#define ACTOR 8

/* Drives actor 8 through a series of position/threshold setup calls and
 * advances the shared scene phase before the scene runs. */
void FieldScene_RunActorEightApproach(void)
{
    u32 i;
    s32 actor;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(-1, -1, -1, 0);
    Func_02007490(1);
    Object_SetModeById_1(ACTOR, 2);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(ACTOR, 0x13080000, 0x3280000);
    actor = Scene_GetRecord_1(ACTOR);
    {
        /* Write 0xa000 to the halfword at +6 of the actor record. */
        s32 value = 0xa000;

        *(u16 *)(actor + 6) = value;
    }
    Func_020074b6(1);
    RatioHistory_RecordAndScheduleCallback_1(0x13333, 1);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(0, 0, 0);
    ObjectMotion_SetPositionAndReset_1(ACTOR, 1);
    Func_020074d6(1);
    SCENE_PHASE = 0x100;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1(ACTOR, 0x6666, 0x3333);
    ObjectMotion_SetPositionAndCommit_1(ACTOR, 0x12d8, 0x2c8);
    ObjectMotion_SetPositionAndCommit_2(ACTOR, 0x12a8, 0x268);
    ObjectMotion_SetSpeedParameters_2(ACTOR, 0x4ccc, 0x2666);
    ObjectMotion_SetPositionAndCommit_3(ACTOR, 0x12a8, 0x1d8);
    ObjectMotion_SetSpeedParameters_3(ACTOR, 0x3333, 0x1999);
    ObjectMotion_SetPositionAndCommit_4(ACTOR, 0x1298, 0x1c8);
    ObjectMotion_SetSpeedParameters_4(ACTOR, 0x1999, 0xccc);
    ObjectMotion_SetPositionAndCommit_5(ACTOR, 0x1298, 0x1b8);
    Object_SetModeById_2(ACTOR, 1);
    BattleRuntime_WaitIfModeZero_2(40);
    Func_0200781e();
    ObjectMotion_SetSpeedParameters_5();
    Func_020077f0(110);
}
