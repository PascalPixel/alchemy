#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001d40[];
void Func_02001c00();
void Func_020046f0();
void Func_0200470a();
void Func_02004724();
void Func_02004740();
void Func_0200474e();
void Func_0200475e();
void Func_02004764();
void Func_0200477a();
void Func_02004790();
void Func_02004796();
void Func_020047aa();
void Func_020047b8();
void Func_020047da();
void Func_0200480a();
void Func_02004822();
void Func_0200484a();
s32 Func_02004868();
void Func_02004880();
void Func_02004896();
void Func_0200489c();
void Func_020048d2();
void Func_020048fa();
void Func_0200497e();
void Func_02004a04();
s32 Func_02004f7e();
void Func_02004fa8();
void Func_02004fd4();
void Func_02005066();
void Func_0200508c();
void Func_020050a4();
void Func_020050aa();
void Func_020050ba();
void Func_020050d4();
void Func_020050de();
void Func_020050e0();
void Func_020050f8();
void Func_020050fc();
void Func_02005104();
void Func_02005106();
void Func_02005114();
void Func_0200511a();
void Func_02005142();
void Func_02005144();
void Func_02005156();
void Func_0200515c();
void Func_02005166();
void Func_02005168();
void Func_0200519c();
void Func_020051a4();
void Func_020051bc();
void Func_020051c4();
void Func_020051e4();
void Func_020051e8();
void Func_020051ec();
void Func_02005210();
void Func_02005214();
void Func_02005218();
void Func_0200521a();
void Func_0200522e();
void Func_0200523e();
void Func_02005244();
void Func_0200524e();
s32 Func_0200525e();
void Func_02005260();
void Func_0200526e();
s32 Func_02005270();
void Func_02005272();
void Func_02005284();
void Func_02005288();
void Func_02005292();
void Func_0200529c();
void Func_020052a6();
void Func_020052be();
u8 *Scene_GetRecord_1();
void Func_020052e4();
void Func_020052ee();
void Func_020052f6();
void Func_0200530c();
void Func_02005314();
void Func_0200531e();
void Func_02005322();
u8 *Scene_GetRecord_2();
void Func_02005338();
void Func_0200533c();
void Func_02005340();
s32 Func_02005342();
void Func_0200534c();
void Func_02005358();
void Func_02005362();
void Func_02005376();
void Func_02005378();
void Func_02005390();
void Func_020053a0();
void Func_020053bc();
void Func_020053d4();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_IsSet_1(a0) Value1(Func_02004f7e, a0)
#define BattleRuntime_Reset_1(args...) Func_02004fa8(args)
#define ObjectMotion_SetAngleToward_1(args...) Func_02005066(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_020050de, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_020050f8, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02005104(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02004fd4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_0200508c(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020050aa, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_020050fc, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(args...) Func_020050a4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_020050ba(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02005106, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_020050d4(args)
#define Object_SetModeById_1(args...) Func_020050e0(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02005168, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_0200515c, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_0200511a(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_0200519c, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_2(args...) Func_02005144(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02005142(args)
#define Object_SetModeById_2(args...) Func_02005156(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02005166(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_020050d4(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_020051e8, a0, a1, a2)
#define SceneWork_SetStepValue_2(args...) Func_020051bc(args)
#define ObjectMotion_SetVariantCallback_3(args...) Func_020051a4(args)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_02005214, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_4(args...) Func_020051bc(args)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_0200522e, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02005210(args)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3(Func_02005244, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_020051ec(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(args...) Func_0200523e(args)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Call3(Func_02005272, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_5(args...) Func_0200521a(args)
#define Object_SetModeById_3(args...) Func_02005218(args)
#define ObjectMotion_ArmCallback_3(args...) Func_02005288(args)
#define ObjectMotion_ArmCallback_4(args...) Func_02005292(args)
#define ObjectMotion_ArmCallback_5(args...) Func_0200529c(args)
#define ObjectMotion_ArmCallback_6(args...) Func_020052a6(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_0200526e(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_020051c4(args)
#define BattleRuntime_WaitIfModeZero_4(a0, a1) Call2(Func_020052e4, a0, a1)
#define BattleRuntime_WaitIfModeZero_5(a0, a1) Call2(Func_020052ee, a0, a1)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_020051e4(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02005284(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_2(a0, a1) Call2(Func_02005322, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_0200533c, a0, a1, a2, a3)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_0200524e, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_0200525e, a0, a1)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02005260, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Func_02005270, a0, a1)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02005272, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020052be, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_6(args...) Func_020052f6(args)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Call3(Func_02005362, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_3(args...) Func_0200534c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_0200530c(args)
#define ObjectMotion_Launch_1(args...) Func_0200531e(args)
#define ObjectMotion_ArmCallback_7(args...) Func_02005378(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_3(a0, a1) Call2(Func_020053bc, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_3(a0, a1, a2, a3) Call4(Func_020053d4, a0, a1, a2, a3)
#define Scene_GetRecord_1(args...) Func_020052d2(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_020052f6, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02005340, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_0200534c, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_02005358, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(args...) Func_020053a0(args)
#define ObjectMotion_SetPositionAndReset_5(a0, a1, a2) Call3(Func_02005376, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02005390(args)
#define Scene_GetRecord_2(args...) Func_02005336(args)
#define Scene_GetRecord_3(a0) Value1(Func_02005342, a0)
#define GameFlag_Set_1(a0) Call1(Func_02005314, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02005338(args)
u8 *Func_020052d2();
u8 *Func_02005336();

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Gated on scene condition 0x911; when set, configures actors 20, 22 and
 * 23 (position, pose, movement and sprite flags) and their attached
 * effects, then advances the shared scene phase. */
void FieldScene_RunActorAndEffectPresentationSetup(void)
{
    u32 i;
    u8 *record;

    if (GameFlag_IsSet_1(0x911) == 0) {
    } else {
        BattleRuntime_Reset_1();
        Func_02005114();
        ObjectMotion_SetAngleToward_1(0, 20, 10);
        ObjectMotion_SetSpeedLimitAndAcceleration_1(0x19999, 0x3333);
        ObjectMotion_PlaceWithinCameraBounds_1(0xbe0000, -1, 0x2c40000, 1);
        Object_CommitPositionThenWaitIfModeZero_1();
        BattleRuntime_WaitIfModeZero_1(40);
        ObjectMotion_SetVariantCallbackAndInvokeObject_1(22, 1);
        SceneWork_SetStepValue_1(0x1d26);
        Func_020046f0(0x4016);
        BattleEffect_SpawnLinkedResourceObject_1(20, 0x102, 60);
        ObjectMotion_SetVariantCallback_1(20, 2);
        Func_0200470a(20);
        ObjectMotion_SetVariantCallbackAndInvokeObject_2(22, 1);
        ObjectMotion_ArmCallback_1(22, 0x5000, 0);
        Func_02004724(0x4016);
        ObjectMotion_SetVariantCallbackAndInvokeObject_3(20, 1);
        Func_0200474e(20, 0xb000);
        Func_02004740(20);
        Func_02004764(23, 0x3000);
        Object_SetModeById_1(23, 3);
        Func_0200475e(0x4017);
        BattleEffect_SpawnLinkedResourceObject_2(22, 0x101, 40);
        ObjectMotion_ArmCallback_2(22, 0x8000, 20);
        Func_0200477a(0x4016);
        Func_02004796(23, 0);
        ObjectMotion_CallThenWaitForAnimationChange_1(23, 4);
        Func_02004790(0x4017);
        BattleEffect_SpawnLinkedResourceObject_3(20, 0x100, 40);
        ObjectMotion_SetVariantCallback_2(20, 2);
        Func_020047aa(20);
        ObjectMotion_CallThenWaitForAnimationChange_2(22, 3);
        Func_020047b8(0x4016);
        Func_020047da(20, 0xd000);
        Object_SetModeById_2(23, 3);
        ObjectMotion_CallThenWaitForAnimationChange_3(20, 3);
        BattleRuntime_WaitIfModeZero_2(60);
        BattleEffect_SpawnLinkedResourceObject_4(22, 0x106, 40);
        Func_0200480a(22, 0x5000);
        SceneWork_SetStepValue_2((s32)Data_00001d40);
        ObjectMotion_SetVariantCallback_3(22, 1);
        Func_0200480a(0x4016);
        BattleEffect_SpawnLinkedResourceObject_5(20, 0x101, 40);
        ObjectMotion_SetVariantCallback_4(20, 2);
        Func_02004822(20);
        BattleEffect_SpawnLinkedResourceObject_6(22, 0x108, 20);
        BattleRuntime_RunThenWaitIfModeZero_1(0x4016, 0, 20);
        BattleEffect_SpawnLinkedResourceObject_7(23, 0x102, 60);
        Func_0200484a(0x4017);
        Value2(Func_02004868, 22, 0x8000);
        ObjectMotion_CallThenWaitForAnimationChange_4(22, 3);
        BattleRuntime_RunThenWaitIfModeZero_2(0x4016, 0, 20);
        BattleEffect_SpawnLinkedResourceObject_8(20, 0x102, 40);
        ObjectMotion_SetVariantCallback_5(20, 2);
        Func_02004880(20);
        Func_0200489c(22, 0x5000);
        Object_SetModeById_3(22, 4);
        Func_02004896(22);
        ObjectMotion_ArmCallback_3(20, 0xb000, 0);
        ObjectMotion_ArmCallback_4(23, 0x3000, 40);
        ObjectMotion_ArmCallback_5(23, 0, 0);
        ObjectMotion_ArmCallback_6(20, 0xd000, 20);
        ObjectMotion_SetVariantCallbackAndInvokeObject_4(22, 2);
        BattleRuntime_WaitIfModeZero_3(20);
        Func_020048d2(0x4016);
        BattleRuntime_WaitIfModeZero_4(23, 0x102);
        BattleRuntime_WaitIfModeZero_5(20, 0x102);
        BattleRuntime_WaitIfModeZero_6(40);
        ObjectMotion_CallThenWaitForAnimationChange_5(22, 3);
        Func_020048fa(0x4016);
        ObjectMotion_SetSpeedLimitAndAcceleration_2(0xcccc, 0x1999);
        ObjectMotion_PlaceWithinCameraBounds_2(0xb60000, -1, 0x2f80000, 1);
        ObjectMotion_SetSpeedParameters_1(23, 0xcccc, 0x6666);
        ObjectMotion_EnableActionAndSetCallback_1(23, 0x200c464);
        ObjectMotion_SetSpeedParameters_2(22, 0xcccc, 0x6666);
        ObjectMotion_EnableActionAndSetCallback_2(22, 0x200c49c);
        ObjectMotion_SetSpeedParameters_3(20, 0xcccc, 0x6666);
        ObjectMotion_SetPositionAndReset_1(20, 182, 0x2f8);
        ObjectMotion_SetVariantCallback_6(20, 2);
        BattleEffect_SpawnLinkedResourceObject_9(20, 0x100, 60);
        Func_0200497e(20, 0xd000);
        BattleRuntime_RunThenWaitIfModeZero_3(20, 0, 20);
        ObjectMotion_CallThenWaitForAnimationChange_6(20, 3);
        ObjectMotion_Launch_1(20, 4, 0);
        ObjectMotion_ArmCallback_7(20, 0x3000, 40);
        ObjectMotion_SetSpeedLimitAndAcceleration_3(0x10000, 0x2000);
        ObjectMotion_PlaceWithinCameraBounds_3(0xd80000, -1, 0x3160000, 1);
        {
            /* Set the low bit of the flag byte at +35 of actor 20's record. */
            u8 *record = Scene_GetRecord_1(20);
            u8 flags = *(volatile u8 *)&record[35];

            record[35] = (u8)(flags | 1);
        }
        ObjectMotion_SetSpeedParameters_4(20, 0x13333, 0x9999);
        ObjectMotion_SetPositionAndReset_2(20, 182, 0x30e);
        ObjectMotion_SetPositionAndReset_3(20, 192, 0x328);
        ObjectMotion_SetPositionAndReset_4(20, 216, 0x328);
        Func_02004a04(20, 0xd000);
        ObjectMotion_SetVariantCallbackAndInvokeObject_5(20, 2);
        Func_02001c00();
        ObjectMotion_SetPositionAndReset_5(20, 216, 0x31e);
        ObjectMotion_SetHorizontalPositionWithTerrain_1(20, 0, 0);
        /* Set the fixed-point word at +24 of actor 20's record to 1.0. */
        record = Scene_GetRecord_2(20);
        *(s32 *)(record + 24) = 0x10000;
        /* Set the fixed-point word at +28 of actor 20's record to 1.0. */
        record = Scene_GetRecord_3(20);
        *(s32 *)(record + 28) = 0x10000;
        GameFlag_Set_1(0x920);
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    }
}
