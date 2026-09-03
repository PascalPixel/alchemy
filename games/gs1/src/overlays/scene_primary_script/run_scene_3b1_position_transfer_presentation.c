#include "types.h"

#define FieldScene_RunPositionTransferPresentation Func_020050e4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001e46[];
extern u8 Data_0200e7c8[];
void Func_020099a6();
void Func_02009ae0();
void Func_02009ae8();
void Func_02009b00();
void Func_02009b1a();
s32 Func_02009b26();
void Func_02009b3a();
void Func_02009ba6();
void Func_0200b5b6();
void Func_0200b60c();
s32 Func_0200b642();
void Func_0200b648();
void Func_0200b64e();
void Func_0200b654();
s32 Func_0200b656();
void Func_0200b660();
s32 Func_0200b66a();
void Func_0200b66c();
void Func_0200b698();
void Func_0200b6ae();
void Func_0200b6b0();
void Func_0200b6b2();
void Func_0200b6c4();
void Func_0200b6c8();
void Func_0200b6d4();
void Func_0200b6d8();
void Func_0200b6e8();
void Func_0200b6ea();
void Func_0200b70c();
void Func_0200b724();
void Func_0200b734();
void Func_0200b73a();
void Func_0200b73c();
void Func_0200b75c();
void Func_0200b76e();
void Func_0200b774();
void Func_0200b798();
s32 Func_0200b7a0();
void Func_0200b7b8();
void Func_0200b7c8();
void Func_0200b7d4();
void Func_0200b7dc();
void Func_0200b7e0();
void Func_0200b7ee();
void Func_0200b816();
void Func_0200b83e();
void Func_0200b84e();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_0200b64e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_0200b73a(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_0200b60c, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_0200b66c(args)
#define ObjectMotion_SetPositionAndCommit_1(a0, a1, a2) Call3(Func_0200b648, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_2(a0, a1, a2) Call3(Func_0200b654, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_3(a0, a1, a2) Call3(Func_0200b660, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_0200b698(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_0200b724, a0, a1, a2)
#define Scene_GetRecord_1(a0) Value1(Func_0200b642, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_0200b6b0(args)
#define Scene_GetRecord_2(a0) Value1(Func_0200b656, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_0200b6c4(args)
#define Scene_GetRecord_3(a0) Value1(Func_0200b66a, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_0200b6d8(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_0200b698, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_0200b6d4, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_0200b6ae, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Call3(Func_0200b6ea, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_0200b6c8, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_0200b70c, a0, a1, a2)
#define Object_SetModeById_3(args...) Func_0200b734(args)
#define Object_SetModeById_4(args...) Func_0200b73c(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_0200b7c8, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_0200b7d4, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_0200b7e0, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_0200b6e8(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_0200b83e, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_0200b7dc(args)
#define SceneWork_SetStepValue_1(args...) Func_0200b816(args)
#define ObjectMotion_ArmCallback_5(args...) Func_0200b84e(args)
#define ObjectMotion_EnableActionAndSetCallback_1(args...) Func_0200b798(args)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Func_0200b7a0, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(args...) Func_0200b7b8(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_0200b76e(args)
#define GameFlag_Set_1(a0) Call1(Func_0200b75c, a0)
#define GameFlag_Clear_1(a0) Call1(Func_0200b774, a0)

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
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

/* Sets up three actor slots (2, 1, 3) with position/pose data pulled from a
 * per-slot lookup record (fields at +8 and +16), then drives a chain of
 * actor animation, camera, and text/dialog calls for the scene. */
void FieldScene_RunPositionTransferPresentation(void)
{
    u32 i;
    /* Per-slot lookup record; fields at +8 and +16 feed the setup call. */
    s32 record;
    s32 base5_1e46;
    s32 base5_200e7c8;

    Func_020099a6(0x1b80000, -1, 0xb00000, 0x1000001);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(0, 0x1b80000, 0x860000);
    BattleRuntime_WaitIfModeZero_1(); /* main:0808a360 */
    ObjectMotion_SetSpeedParameters_1(0, 0x19999, 0xcccc);
    Object_SetModeById_1(0, 5);
    ObjectMotion_SetPositionAndCommit_1(0, 0x198, 134);
    ObjectMotion_SetPositionAndCommit_2(0, 0x198, 152);
    ObjectMotion_SetPositionAndCommit_3(0, 0x1b0, 166);
    Object_SetModeById_2(0, 1);
    ObjectMotion_ArmCallback_1(0, 0x4000, 0);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_2(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_3(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_3(1);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_4(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_0200b5b6(1); /* main:080000c0 */
    ObjectMotion_SetSpeedParameters_2(2, 0x19999, 0xcccc);
    ObjectMotion_ResetAndSetPositionInMode2_1(2, 0x1a8, 152);
    ObjectMotion_SetSpeedParameters_3(1, 0x19999, 0xcccc);
    ObjectMotion_ResetAndSetPositionInMode2_2(1, 0x1c0, 168);
    ObjectMotion_SetSpeedParameters_4(3, 0x20000, 0x10000);
    ObjectMotion_SetPositionAndReset_1(3, 0x1ca, 152);
    Object_SetModeById_3(1, 1);
    Object_SetModeById_4(2, 1);
    ObjectMotion_ArmCallback_2(2, 0x4000, 0);
    ObjectMotion_ArmCallback_3(1, 0x4000, 0);
    ObjectMotion_ArmCallback_4(3, 0x4000, 40);
    Func_0200b7ee(10); /* main:0808a1d8 */
    /* Text/dialog resource pointer, passed by base address and by base+1. */
    base5_1e46 = (s32)Data_00001e46;
    Func_0200b6b2(base5_1e46, 1, 10); /* main:08015210 */
    BattleRuntime_WaitIfModeZero_2(10);
    Func_02009b1a(0, 0, 40);
    Value3(Func_02009b26, 1, 0x4000, 20);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x39999, 0x7333); /* main:0808a208 */
    Call4(Func_02009b00, 0x1b80000, -1, 0x1400000, 0x10000014);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 2); /* main:0808a138 */
    Call2(Func_02009ae8, 8, 0xd000);
    SceneWork_SetStepValue_1((base5_1e46 + 1)); /* main:0808a170 */
    Func_02009ae0(8);
    ObjectMotion_ArmCallback_5(8, 0, 20);
    Call4(Func_02009b3a, 0x1b80000, -1, 0x860000, 0x10000000);
    /* Second text/dialog resource pointer, shared across three calls. */
    base5_200e7c8 = (s32)Data_0200e7c8;
    ObjectMotion_EnableActionAndSetCallback_1(1, base5_200e7c8);
    ObjectMotion_EnableActionAndSetCallback_2(2, base5_200e7c8);
    ObjectMotion_MarkActiveAndSetActionCallback_1(3, base5_200e7c8); /* main:0808a0b0 */
    BattleRuntime_WaitIfModeZero_3(40);
    GameFlag_Set_1(0x301);
    Func_02009ba6(23, 0, 0);
    GameFlag_Clear_1(0x12f);
}
