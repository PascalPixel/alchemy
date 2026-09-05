#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200572a();
void Func_02005734();
void Func_02005738();
void Func_02005770();
void Func_02005780();
u8 *Scene_GetRecord_1();
void Func_020057a4();
void Func_020057b8();
void Func_020057ba();
void Func_020057c0();
void Func_020057ca();
void Func_020057ea();
void Func_020057f2();
void Func_02005804();
void Func_0200581e();
void Func_02005828();
void Func_0200583a();
void Func_02005842();
void Func_0200584e();
void Func_02005852();
void Func_02005854();
void Func_0200585a();
void Func_02005864();
void Func_0200586e();
void Func_02005876();
void Func_02005886();
void Func_0200588e();
void Func_02005892();
void Func_020058c2();
void Func_020058cc();
void Func_020058d6();
void Func_020058d8();
void Func_020058da();
void Func_020058e2();
void Func_020058ee();
void Func_020058fa();
void Func_0200590a();
void Func_02005916();
void Func_02005932();
void Func_0200593e();
void Func_02005994();
void Func_020059a0();
void Func_020059a2();
void Func_020059ac();
void Func_020059b8();
void Func_020059f4();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1(args...) Func_02005876(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02005734, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02005770, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02005804, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_020057a4(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_0200572a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_020057ca(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02005738(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_0200583a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02005780, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_020057ba, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_0200584e, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02005794(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_020057ea(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_020057b8, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_020057f2, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02005886, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02005892, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02005842(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02005852(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_020057c0(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020058c2, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_020058cc, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02005854(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02005864(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_0200581e, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02005828, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_0200585a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_0200586e, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_0200588e(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_0200590a, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02005916, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02005932, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_0200593e, a0, a1, a2)
#define ObjectMotion_Launch_1(args...) Func_020058d8(args)
#define ObjectMotion_Launch_2(args...) Func_020058e2(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_020058d6, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_020058e2, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(a0, a1, a2) Call3(Func_020058ee, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(a0, a1, a2) Call3(Func_020058fa, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_020059a2, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_020059b8, a0, a1, a2, a3)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02005994, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_020059a0, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_020059ac, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_020059b8, a0, a1, a2)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_020059f4(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_020058da(args)
u8 *Func_02005794();

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

/* Runs a sequence of position/scale/timing calls for actor pair 0 and 1,
 * copying a stored pair of 32-bit fields (offsets +8, +16) from actor 0's
 * record onto actor 1 partway through, then runs an analogous sequence for
 * actors 5, 9, 10 and 11. */
void FieldScene_StagePairedActors(void)
{
    u32 i;
    u8 *record;

    Audio_PlayCue_1(17);
    ObjectMotion_SetSpeedParameters_1(0, 0x8000, 0x4000);
    ObjectMotion_SetPositionAndReset_1(0, 231, 0x1ea);
    ObjectMotion_ArmCallback_1(0, 0xc000, 30);
    ObjectMotion_CallThenWaitForAnimationChange_1(0, 3);
    BattleRuntime_WaitIfModeZero_1(180);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(0, 2);
    BattleRuntime_WaitIfModeZero_2(80);
    BattleEffect_SpawnLinkedResourceObject_1(0, 0x101, 60);
    ObjectMotion_SetSpeedParameters_2(0, 0x8000, 0x4000);
    ObjectMotion_SetPositionAndReset_2(0, 246, 0x1df);
    ObjectMotion_ArmCallback_2(0, 0xe000, 10);
    /* Copy actor 0's stored fields at +8 and +16 onto actor 1, if a record
     * for actor 0 exists. */
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_1(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    ObjectMotion_SetSpeedParameters_3(1, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset_3(1, 0x101, 0x1eb);
    ObjectMotion_ArmCallback_3(0, 0x2000, 0);
    ObjectMotion_ArmCallback_4(1, 0xa000, 40);
    ObjectMotion_SetVariantCallback_1(0, 2);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 2);
    BattleRuntime_WaitIfModeZero_3(20);
    BattleEffect_SpawnLinkedResourceObject_2(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_3(1, 0x101, 80);
    Object_SetModeById_1(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_2(1, 4);
    ObjectMotion_SetSpeedParameters_4(0, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_5(1, 0x13333, 0x9999);
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 0x109, 0x1c5);
    ObjectMotion_SetPositionAndReset_4(1, 0x11a, 0x1d5);
    Object_SetModeById_2(0, 1);
    ObjectMotion_ArmCallback_5(0, 0xe000, 0);
    ObjectMotion_ArmCallback_6(1, 0xe000, 40);
    BattleEffect_SpawnLinkedResourceObject_4(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_5(1, 0x100, 0);
    ObjectMotion_Launch_1(0, 6, 0);
    ObjectMotion_Launch_2(1, 6, 60);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(5, 0x1db0000, 0x14c0000);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(9, 0x1eb0000, 0x14c0000);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(11, 0x1cb0000, 0x15c0000);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(10, 0x1fb0000, 0x15c0000);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x73333, 0xe666);
    ObjectMotion_PlaceWithinCameraBounds_1(0x1e50000, -1, 0x1590000, 1);
    ObjectMotion_ArmCallback_7(5, 0x6000, 0);
    ObjectMotion_ArmCallback_8(9, 0x5000, 0);
    ObjectMotion_ArmCallback_9(11, 0x5000, 0);
    ObjectMotion_ArmCallback_10(10, 0x5000, 0);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleRuntime_WaitIfModeZero_4(40);
}
