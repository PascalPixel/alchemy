#include "types.h"

#define FieldScene_RunScene3c6SequenceA Func_02001090

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_020025c4();
void Func_020025d2();
void Func_020025e0();
void Func_020025ee();
void Func_020025fc();
void Func_02002632();
void Func_02002640();
void Func_0200264e();
void Func_02002654();
void Func_0200265c();
void Func_0200265e();
void Func_02002660();
void Func_0200266a();
void Func_02002684();
void Func_0200268a();
void Func_02002692();
void Func_020026a0();
void Func_020026a6();
void Func_020026b2();
void Func_020026c2();
void Func_020026c4();
void Func_020026ce();
void Func_020026d0();
void Func_020026d2();
void Func_020026dc();
void Func_020026e8();
void Func_020026f4();
void Func_020026f8();
void Func_02002704();
void Func_02002714();
void Func_0200272e();
void Func_0200273e();
void Func_02002744();
void Func_0200274a();
void Func_02002764();
void Func_02002776();
void Func_0200278a();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define SceneWork_SetStepValue_1(a0) Call1(Func_0200265e, a0)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020025c4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02002654(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_020025d2(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_0200268a(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_020025e0(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02002660(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_020025ee(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_020026a6(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_020025fc(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_020026d0, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020026dc, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_020026e8, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_020026f4, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02002632(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_020026c2(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02002640(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_020026f8(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_0200264e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_020026ce(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_0200265c(args)
#define BattleEvent_RunActionAndWait_4(args...) Func_02002714(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_0200266a(args)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_0200273e, a0, a1, a2)
#define BattleEvent_RunActionAndWait_5(args...) Func_0200272e(args)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_02002684(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02002704(args)
#define BattleRuntime_WaitIfModeZero_12(args...) Func_02002692(args)
#define BattleEvent_RunActionAndWait_6(args...) Func_0200274a(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_020026a0(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02002764, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_020026b2(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02002776, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_020026c4(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_02002744(args)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_020026d2(args)
#define BattleEvent_RunActionAndWait_7(args...) Func_0200278a(args)

/* Sets the s16 field at offset 0x1d8 of the shared scene work record
 * (the scene step counter, per bump_step below) directly to a value. */

/* Calls ObjectMotion_SetVariantCallback(), then forwards arg0 into an
 * unnamed follow-up call. */

/* Calls an unnamed function, then ObjectMotion_WaitForAnimationChange(arg0). */

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Runs a fixed, unbranching sequence of overlay calls with constant
 * arguments; no loop, no stored result, no use of the scene work record. */
void FieldScene_RunScene3c6SequenceA(void)
{
    u32 i;
    u8 *record;

    SceneWork_SetStepValue_1(0x28b0);
    BattleRuntime_WaitIfModeZero_1(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(22, 2);
    BattleRuntime_WaitIfModeZero_2(20);
    BattleEvent_RunActionAndWait_1(22, 0);
    BattleRuntime_WaitIfModeZero_3(10);
    ObjectMotion_CallThenWaitForAnimationChange_1(22, 4);
    BattleRuntime_WaitIfModeZero_4(20);
    BattleEvent_RunActionAndWait_2(22, 0);
    BattleRuntime_WaitIfModeZero_5(10);
    BattleEffect_SpawnLinkedResourceObject_1(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_3(3, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_4(2, 0x100, 55);
    BattleRuntime_WaitIfModeZero_6(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 2);
    BattleRuntime_WaitIfModeZero_7(20);
    BattleEvent_RunActionAndWait_3(1, 0);
    BattleRuntime_WaitIfModeZero_8(10);
    ObjectMotion_CallThenWaitForAnimationChange_2(22, 4);
    BattleRuntime_WaitIfModeZero_9(20);
    BattleEvent_RunActionAndWait_4(22, 0);
    BattleRuntime_WaitIfModeZero_10(10);
    BattleEffect_SpawnLinkedResourceObject_5(3, 0x102, 40);
    BattleEvent_RunActionAndWait_5(3, 0);
    BattleRuntime_WaitIfModeZero_11(10);
    ObjectMotion_CallThenWaitForAnimationChange_3(1, 3);
    BattleRuntime_WaitIfModeZero_12(30);
    BattleEvent_RunActionAndWait_6(1, 0);
    BattleRuntime_WaitIfModeZero_13(10);
    ObjectMotion_ArmCallback_1(2, 0x8000, 0);
    BattleRuntime_WaitIfModeZero_14(65);
    ObjectMotion_ArmCallback_2(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_15(40);
    ObjectMotion_CallThenWaitForAnimationChange_4(2, 3);
    BattleRuntime_WaitIfModeZero_16(30);
    BattleEvent_RunActionAndWait_7(2, 0);
}
