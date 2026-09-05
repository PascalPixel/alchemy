#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_020061c6();
s32 Func_020061d0();
s32 Func_020061d8();
s32 Func_020061e0();
s32 Func_020061e8();
void Func_02006234();
void Func_0200623e();
s32 Func_02006242();
void Func_02006260();
void Func_02006272();
void Func_02006288();
void Func_020062ac();
void Func_020062b2();
void Func_020062b6();
void Func_020062ba();
void Func_020062ca();
void Func_020062d4();
void Func_020062dc();
void Func_020062de();
void Func_020062e8();
void Func_020062ee();
void Func_020062f2();
void Func_020062f8();
void Func_020062fc();
void Func_02006304();
void Func_02006306();
void Func_0200630e();
void Func_0200631a();
void Func_02006332();
void Func_02006340();
void Func_02006350();
void Func_02006372();
void Func_0200637c();
void Func_02006392();
void Func_020063a0();
void Func_020063a8();
void Func_020063b2();
void Func_020063b4();
void Func_020063bc();
void Func_020063c2();
void Func_020063cc();
void Func_020063d0();
void Func_020063d2();
void Func_020063e6();
void Func_020063fc();
void Func_02006404();
void Func_0200641c();
void Func_0200642a();
void Func_0200643c();
void Func_02006448();
void Func_0200644c();
void Func_0200644e();
void Func_02006452();
void Func_02006456();
void Func_02006476();
void Func_0200647e();
void Func_02006480();
void Func_02006486();
void Func_0200648e();
void Func_02006492();
void Func_02006496();
void Func_02006498();
void Func_0200649e();
void Func_020064a6();
void Func_020064ae();
void Func_020064b6();
void Func_020064be();
void Func_020064c4();
void Func_020064c6();
void Func_020064d6();
void Func_020064e6();
void Func_02006506();
void Func_0200650e();
void Func_02006526();
void Func_02006534();
void Func_02006542();
s32 Func_0200654e();
void Func_02006568();
void Func_02006570();
void Func_02006578();
void Func_02006588();
void Func_0200658e();
void Func_02006598();
void Func_020065a8();
void Func_020065b2();
void Func_020065b8();
void Func_020065c4();
void Func_020065c8();
void Func_020065ca();
void Func_020065d6();
void Func_020065d8();
void Func_020065f0();
void Func_02006604();
void Func_02006606();
void Func_02006610();
void Func_02006612();
void Func_0200661a();
void Func_02006622();
void Func_02006628();
void Func_0200662c();
void Func_02006636();
void Func_02006640();
void Func_0200664a();
void Func_02006652();
void Func_02006654();
void Func_0200665a();
void Func_02006664();
void Func_0200667a();
void Func_02006682();
void Func_0200668a();
void Func_02006692();
void Func_0200669a();
void Func_0200669c();
void Func_020066a2();
void Func_020066aa();
void Func_020066b0();
void Func_020066cc();
void Func_020066da();
void Func_020066dc();
void Func_020066e4();
void Func_020066ec();
void Func_020066f4();
void Func_020066fc();
void Func_02006704();
void Func_0200670c();
void Func_02006714();
s32 Func_02006724();
void Func_02006734();
void Func_02006744();
void Func_0200674a();
void Func_0200674c();
void Func_0200675c();
void Func_02006786();
void Func_0200678e();
void Func_02006796();
void Func_0200679a();
void Func_0200679e();
void Func_020067a4();
void Func_020067a6();
void Func_020067ae();
void Func_020067b0();
void Func_020067b6();
void Func_020067c6();
void Func_020067ce();
s32 Func_020067d0();
void Func_020067d2();
void Func_020067d6();
void Func_020067de();
void Func_020067f0();
void Func_020067f2();
void Func_020067fa();
void Func_020067fc();
s32 Func_020067fe();
void Func_02006806();
s32 Func_0200680a();
void Func_0200680c();
void Func_0200680e();
s32 Func_02006816();
void Func_02006840();
void Func_0200685a();
void Func_02006864();
void Func_020068a2();
void Func_02006900();
void Func_0200699c();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Scene_GetRecord_1(args...) Func_020061c6(args)
#define Scene_GetRecord_2(a0) Value1(Func_020061d0, a0)
#define Scene_GetRecord_3(a0) Value1(Func_020061d8, a0)
#define Scene_GetRecord_4(a0) Value1(Func_020061e0, a0)
#define Scene_GetRecord_5(args...) Func_020061e8(args)
#define Object_SetModeById_1(a0, a1) Value2(Func_02006242, a0, a1)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_020062de, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_020062f8, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02006304(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020062b2, a0)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02006234, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_0200623e, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(args...) Func_02006260(args)
#define ObjectMotion_SetPositionAndReset_1(args...) Func_02006272(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02006288(args)
#define ObjectMotion_SetAngleToward_1(args...) Func_020062ca(args)
#define ObjectMotion_SetAngleToward_2(args...) Func_020062d4(args)
#define ObjectMotion_SetAngleToward_3(args...) Func_020062de(args)
#define ObjectMotion_SetAngleToward_4(args...) Func_020062e8(args)
#define ObjectMotion_SetAngleToward_5(args...) Func_020062f2(args)
#define ObjectMotion_SetAngleToward_6(args...) Func_020062fc(args)
#define ObjectMotion_SetAngleToward_7(args...) Func_02006306(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_020062ac, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_020062ba, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_2(args...) Func_020062dc(args)
#define ObjectMotion_SetPositionAndReset_2(args...) Func_020062ee(args)
#define ObjectMotion_SetAngleToward_8(args...) Func_02006340(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_0200630e(args)
#define ObjectMotion_SetAngleToward_9(args...) Func_02006350(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02006350(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020062b6(args)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_020063a0(args)
#define ObjectMotion_SetAngleToward_10(args...) Func_02006372(args)
#define ObjectMotion_SetAngleToward_11(args...) Func_0200637c(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020063d0, a0, a1, a2)
#define ObjectMotion_SetAngleToward_12(args...) Func_02006392(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020063e6, a0, a1, a2)
#define ObjectMotion_SetAngleToward_13(args...) Func_020063a8(args)
#define ObjectMotion_SetAngleToward_14(args...) Func_020063b2(args)
#define ObjectMotion_SetAngleToward_15(args...) Func_020063bc(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_0200631a(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(args...) Func_02006404(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_020063b4(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02006332(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_020063c2(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02006340(args)
#define BattleRuntime_RunThenWaitIfModeZero_3(args...) Func_0200642a(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_0200644c, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02006456, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_4(args...) Func_02006448(args)
#define BattleRuntime_RunThenWaitIfModeZero_5(args...) Func_02006452(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02006476, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02006480, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_020063d2, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(args...) Func_020063fc(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02006498, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_6(args...) Func_02006492(args)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_020064ae, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_0200644e(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_020063cc(args)
#define BattleRuntime_RunThenWaitIfModeZero_7(args...) Func_020064b6(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02006476(args)
#define ObjectMotion_SetVariantCallback_2(args...) Func_0200647e(args)
#define ObjectMotion_SetVariantCallback_3(args...) Func_02006486(args)
#define ObjectMotion_SetVariantCallback_4(args...) Func_0200648e(args)
#define ObjectMotion_SetVariantCallback_5(args...) Func_02006496(args)
#define ObjectMotion_SetVariantCallback_6(args...) Func_0200649e(args)
#define ObjectMotion_SetVariantCallback_7(args...) Func_020064a6(args)
#define ObjectMotion_SetVariantCallback_8(args...) Func_020064ae(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_0200641c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_020064c4(args)
#define BattleRuntime_RunThenWaitIfModeZero_8(args...) Func_0200650e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_020064be(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_0200643c(args)
#define BattleRuntime_RunThenWaitIfModeZero_9(args...) Func_02006526(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02006542, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_0200654e, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_1(a0, a1) Value2(Func_0200654e, a0, a1)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_02006568, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02006476(args)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_02006578, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02006486(args)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3(Func_02006588, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_02006496(args)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Call3(Func_02006598, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_020064a6(args)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Call3(Func_020065a8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_12(args...) Func_020064b6(args)
#define BattleEffect_SpawnLinkedResourceObject_10(a0, a1, a2) Call3(Func_020065b8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_020064c6(args)
#define BattleEffect_SpawnLinkedResourceObject_11(a0, a1, a2) Call3(Func_020065c8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_020064d6(args)
#define BattleEffect_SpawnLinkedResourceObject_12(a0, a1, a2) Call3(Func_020065d8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_020064e6(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_0200658e(args)
#define BattleRuntime_RunThenWaitIfModeZero_10(args...) Func_020065d8(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02006588(args)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_02006506(args)
#define BattleRuntime_RunThenWaitIfModeZero_11(args...) Func_020065f0(args)
#define BattleEffect_SpawnLinkedResourceObject_13(a0, a1, a2) Call3(Func_02006612, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_12(args...) Func_02006604(args)
#define ObjectMotion_SetAngleToward_16(args...) Func_020065d6(args)
#define BattleRuntime_WaitIfModeZero_17(args...) Func_02006534(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_020065c4(args)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_02006542(args)
#define BattleRuntime_RunThenWaitIfModeZero_13(args...) Func_0200662c(args)
#define Object_LinkPair_1(args...) Func_02006606(args)
#define Object_LinkPair_2(args...) Func_02006610(args)
#define Object_LinkPair_3(args...) Func_0200661a(args)
#define BattleRuntime_WaitIfModeZero_19(args...) Func_02006570(args)
#define ObjectMotion_SetAngleToward_17(args...) Func_02006622(args)
#define ObjectMotion_SetAngleToward_18(args...) Func_0200662c(args)
#define ObjectMotion_SetAngleToward_19(args...) Func_02006636(args)
#define ObjectMotion_SetAngleToward_20(args...) Func_02006640(args)
#define ObjectMotion_SetAngleToward_21(args...) Func_0200664a(args)
#define ObjectMotion_SetAngleToward_22(args...) Func_02006654(args)
#define BattleRuntime_WaitIfModeZero_20(args...) Func_020065b2(args)
#define BattleRuntime_RunThenWaitIfModeZero_14(args...) Func_0200669c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_02006664(args)
#define BattleRuntime_WaitIfModeZero_21(args...) Func_020065ca(args)
#define Object_SetModeById_2(args...) Func_02006652(args)
#define Object_SetModeById_3(args...) Func_0200665a(args)
#define Object_SetModeById_4(args...) Func_0200667a(args)
#define Object_SetModeById_5(args...) Func_02006682(args)
#define Object_SetModeById_6(args...) Func_0200668a(args)
#define Object_SetModeById_7(args...) Func_02006692(args)
#define Object_SetModeById_8(args...) Func_0200669a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_020066aa(args)
#define BattleRuntime_WaitIfModeZero_22(args...) Func_02006628(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02006724, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_2(a0, a1) Value2(Func_02006724, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_8(args...) Func_020066cc(args)
#define BattleRuntime_WaitIfModeZero_23(args...) Func_0200664a(args)
#define BattleRuntime_RunThenWaitIfModeZero_15(args...) Func_02006734(args)
#define Object_SetModeById_9(args...) Func_020066dc(args)
#define Object_SetModeById_10(args...) Func_020066e4(args)
#define Object_SetModeById_11(args...) Func_020066ec(args)
#define Object_SetModeById_12(args...) Func_020066f4(args)
#define Object_SetModeById_13(args...) Func_020066fc(args)
#define Object_SetModeById_14(args...) Func_02006704(args)
#define Object_SetModeById_15(args...) Func_0200670c(args)
#define Object_SetModeById_16(args...) Func_02006714(args)
#define ObjectMotion_CallThenWaitForAnimationChange_9(args...) ((void (*)())Func_02006724)(args)
#define BattleRuntime_WaitIfModeZero_24(args...) Func_020066a2(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(args...) Func_0200674a(args)
#define BattleRuntime_WaitIfModeZero_25(args...) Func_020066b0(args)
#define BattleRuntime_RunThenWaitIfModeZero_16(args...) Func_0200679a(args)
#define BattleRuntime_RunThenWaitIfModeZero_17(args...) Func_020067a4(args)
#define Object_SetModeById_17(args...) Func_0200674c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_10(args...) Func_0200675c(args)
#define BattleRuntime_WaitIfModeZero_26(args...) Func_020066da(args)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_020067d6, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(args...) Func_0200678e(args)
#define BattleRuntime_WaitIfModeZero_27(args...) Func_020066f4(args)
#define BattleRuntime_RunThenWaitIfModeZero_18(args...) Func_020067de(args)
#define Object_SetModeById_18(args...) Func_02006786(args)
#define Object_SetModeById_19(args...) Func_0200678e(args)
#define Object_SetModeById_20(args...) Func_02006796(args)
#define Object_SetModeById_21(args...) Func_0200679e(args)
#define Object_SetModeById_22(args...) Func_020067a6(args)
#define Object_SetModeById_23(args...) Func_020067ae(args)
#define Object_SetModeById_24(args...) Func_020067b6(args)
#define ObjectMotion_CallThenWaitForAnimationChange_11(args...) Func_020067c6(args)
#define BattleRuntime_WaitIfModeZero_28(args...) Func_02006744(args)
#define ObjectMotion_ResetAndSetPositionInMode2_3(args...) Func_020067a6(args)
#define ObjectMotion_ResetAndSetPositionInMode2_4(args...) Func_020067b0(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(args...) Func_020067ce(args)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_0200685a, a0, a1, a2)
#define Object_SetModeById_25(args...) Func_020067f2(args)
#define Object_SetModeById_26(args...) Func_020067fa(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(args...) Func_020067f0(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_020067d0, a0, a1)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_020067d2, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4(args...) Func_020067fc(args)
#define ObjectMotion_SetPositionAndReset_5(args...) Func_02006806(args)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_020068a2, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(args...) Func_0200685a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02006840, a0, a1, a2)
#define Scene_GetRecord_6(args...) Func_020067fe(args)
#define Scene_GetRecord_7(args...) Func_0200680a(args)
#define Scene_GetRecord_8(args...) Func_02006816(args)
#define Audio_PlayCue_1(args...) Func_0200699c(args)
#define BattleRuntime_WaitIfModeZero_29(args...) Func_020067fa(args)
#define ObjectMotion_SetPositionAndReset_6(args...) Func_02006864(args)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_02006900, a0, a1, a2)
#define GameFlag_Clear_1(a0) Call1(Func_0200680e, a0)
#define GameFlag_Set_1(a0) Call1(Func_0200680c, a0)

/* Newly named raw call sites: each engine function below was reached only
 * through an unresolved veneer island until its own source was read; the
 * per-site macro keeps the site's original calling form. */

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

/* Field cleared to 0 on each actor record below. */
#define ACTOR_FIELD_108(rec) (*(s32 *)((rec) + 108))

/* Looks up actors 18, 13, 14, 15 and 16 and clears their +108 field before
 * the scene runs. */
void FieldScene_RunEnsembleStoryBeat(void)
{
    u32 i;
    s32 actor;

    actor = Scene_GetRecord_1(18);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_2(13);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_3(14);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_4(15);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_5(16);
    ACTOR_FIELD_108(actor) = 0;
    Object_SetModeById_1(11, 1);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x8000, 0x1000);
    ObjectMotion_PlaceWithinCameraBounds_1(0xe80000, -1, 0xc80000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    SceneWork_SetStepValue_1(0x1883);
    ObjectMotion_SetSpeedParameters_1(10, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_2(12, 0xcccc, 0x6666);
    ObjectMotion_ResetAndSetPositionInMode2_1(10, 152, 200);
    ObjectMotion_SetPositionAndReset_1(12, 144, 248);
    ObjectMotion_CommitCurrentPositionAndActivate_1(10);
    ObjectMotion_SetAngleToward_1(9, 19, 0);
    ObjectMotion_SetAngleToward_2(11, 19, 0);
    ObjectMotion_SetAngleToward_3(13, 19, 0);
    ObjectMotion_SetAngleToward_4(14, 19, 0);
    ObjectMotion_SetAngleToward_5(15, 19, 0);
    ObjectMotion_SetAngleToward_6(16, 19, 0);
    ObjectMotion_SetAngleToward_7(18, 19, 0);
    ObjectMotion_SetSpeedParameters_3(10, 0x18000, 0xc000);
    ObjectMotion_SetSpeedParameters_4(12, 0x20000, 0x10000);
    ObjectMotion_ResetAndSetPositionInMode2_2(10, 152, 200);
    ObjectMotion_SetPositionAndReset_2(12, 144, 248);
    ObjectMotion_SetAngleToward_8(12, 19, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_2(10);
    ObjectMotion_SetAngleToward_9(10, 19, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(18, 2);
    BattleRuntime_WaitIfModeZero_1(20);
    BattleRuntime_RunThenWaitIfModeZero_1(18, 0, 40);
    ObjectMotion_SetAngleToward_10(9, 18, 0);
    ObjectMotion_SetAngleToward_11(10, 18, 0);
    ObjectMotion_ArmCallback_1(11, 0x3000, 0);
    ObjectMotion_SetAngleToward_12(12, 18, 0);
    ObjectMotion_ArmCallback_2(13, 0x3000, 0);
    ObjectMotion_SetAngleToward_13(14, 18, 0);
    ObjectMotion_SetAngleToward_14(15, 18, 0);
    ObjectMotion_SetAngleToward_15(16, 18, 0);
    BattleRuntime_WaitIfModeZero_2(20);
    BattleRuntime_RunThenWaitIfModeZero_2(16, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_1(18, 3);
    BattleRuntime_WaitIfModeZero_3(20);
    ObjectMotion_CallThenWaitForAnimationChange_2(16, 3);
    BattleRuntime_WaitIfModeZero_4(20);
    BattleRuntime_RunThenWaitIfModeZero_3(16, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_1(18, 0x105, 60);
    BattleEffect_SpawnLinkedResourceObject_2(16, 0x101, 60);
    BattleRuntime_RunThenWaitIfModeZero_4(16, 0, 20);
    BattleRuntime_RunThenWaitIfModeZero_5(18, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_3(16, 0x102, 60);
    BattleEffect_SpawnLinkedResourceObject_4(15, 0x101, 60);
    ObjectMotion_SetSpeedParameters_5(15, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_3(15, 216, 176);
    ObjectMotion_ArmCallback_3(15, 0x3000, 20);
    BattleRuntime_RunThenWaitIfModeZero_6(15, 0, 20);
    ObjectMotion_ArmCallback_4(18, 0xb000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_3(18, 4);
    BattleRuntime_WaitIfModeZero_5(20);
    BattleRuntime_RunThenWaitIfModeZero_7(18, 0, 20);
    ObjectMotion_SetVariantCallback_1(9, 2);
    ObjectMotion_SetVariantCallback_2(10, 2);
    ObjectMotion_SetVariantCallback_3(11, 2);
    ObjectMotion_SetVariantCallback_4(12, 2);
    ObjectMotion_SetVariantCallback_5(13, 2);
    ObjectMotion_SetVariantCallback_6(14, 2);
    ObjectMotion_SetVariantCallback_7(15, 2);
    ObjectMotion_SetVariantCallback_8(16, 2);
    BattleRuntime_WaitIfModeZero_6(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(13, 2);
    BattleRuntime_RunThenWaitIfModeZero_8(13, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_4(18, 3);
    BattleRuntime_WaitIfModeZero_7(20);
    BattleRuntime_RunThenWaitIfModeZero_9(18, 0, 20);
    ObjectMotion_ArmCallback_5(0, 0xe000, 20);
    ObjectMotion_ArmCallback_6(18, 0x5000, 20);
    BattleEventRuntime_ProcessAction_1(18, 0);
    BattleEffect_SpawnLinkedResourceObject_5(9, 0x101, 0);
    BattleRuntime_WaitIfModeZero_8(5);
    BattleEffect_SpawnLinkedResourceObject_6(10, 0x101, 0);
    BattleRuntime_WaitIfModeZero_9(5);
    BattleEffect_SpawnLinkedResourceObject_7(11, 0x101, 0);
    BattleRuntime_WaitIfModeZero_10(5);
    BattleEffect_SpawnLinkedResourceObject_8(12, 0x101, 0);
    BattleRuntime_WaitIfModeZero_11(5);
    BattleEffect_SpawnLinkedResourceObject_9(13, 0x101, 0);
    BattleRuntime_WaitIfModeZero_12(5);
    BattleEffect_SpawnLinkedResourceObject_10(14, 0x101, 0);
    BattleRuntime_WaitIfModeZero_13(5);
    BattleEffect_SpawnLinkedResourceObject_11(15, 0x101, 0);
    BattleRuntime_WaitIfModeZero_14(5);
    BattleEffect_SpawnLinkedResourceObject_12(16, 0x101, 0);
    BattleRuntime_WaitIfModeZero_15(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(16, 2);
    BattleRuntime_RunThenWaitIfModeZero_10(16, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_5(18, 3);
    BattleRuntime_WaitIfModeZero_16(20);
    BattleRuntime_RunThenWaitIfModeZero_11(18, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_13(15, 0x101, 60);
    BattleRuntime_RunThenWaitIfModeZero_12(15, 0, 20);
    ObjectMotion_SetAngleToward_16(18, 15, 0);
    BattleRuntime_WaitIfModeZero_17(20);
    ObjectMotion_CallThenWaitForAnimationChange_6(18, 4);
    BattleRuntime_WaitIfModeZero_18(20);
    BattleRuntime_RunThenWaitIfModeZero_13(18, 0, 40);
    Object_LinkPair_1(11, 10, 0);
    Object_LinkPair_2(12, 14, 0);
    Object_LinkPair_3(13, 15, 0);
    BattleRuntime_WaitIfModeZero_19(60);
    ObjectMotion_SetAngleToward_17(10, 18, 0);
    ObjectMotion_SetAngleToward_18(11, 18, 0);
    ObjectMotion_SetAngleToward_19(12, 18, 0);
    ObjectMotion_SetAngleToward_20(13, 18, 0);
    ObjectMotion_SetAngleToward_21(14, 18, 0);
    ObjectMotion_SetAngleToward_22(15, 18, 0);
    BattleRuntime_WaitIfModeZero_20(20);
    BattleRuntime_RunThenWaitIfModeZero_14(18, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(18, 2);
    BattleRuntime_WaitIfModeZero_21(20);
    Object_SetModeById_2(9, 3);
    Object_SetModeById_3(10, 3);
    Object_SetModeById_4(11, 3);
    Object_SetModeById_5(12, 3);
    Object_SetModeById_6(13, 3);
    Object_SetModeById_7(14, 3);
    Object_SetModeById_8(15, 3);
    ObjectMotion_CallThenWaitForAnimationChange_7(16, 3);
    BattleRuntime_WaitIfModeZero_22(20);
    ObjectMotion_ArmCallback_7(18, 0x5000, 20);
    BattleEventRuntime_ProcessAction_2(18, 0);
    ObjectMotion_CallThenWaitForAnimationChange_8(18, 3);
    BattleRuntime_WaitIfModeZero_23(20);
    BattleRuntime_RunThenWaitIfModeZero_15(18, 0, 20);
    Object_SetModeById_9(0, 3);
    Object_SetModeById_10(9, 3);
    Object_SetModeById_11(10, 3);
    Object_SetModeById_12(11, 3);
    Object_SetModeById_13(12, 3);
    Object_SetModeById_14(13, 3);
    Object_SetModeById_15(14, 3);
    Object_SetModeById_16(15, 3);
    ObjectMotion_CallThenWaitForAnimationChange_9(16, 3);
    BattleRuntime_WaitIfModeZero_24(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(18, 2);
    BattleRuntime_WaitIfModeZero_25(20);
    BattleRuntime_RunThenWaitIfModeZero_16(18, 0, 20);
    BattleRuntime_RunThenWaitIfModeZero_17(18, 0, 20);
    Object_SetModeById_17(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_10(18, 3);
    BattleRuntime_WaitIfModeZero_26(20);
    ObjectMotion_ArmCallback_8(18, 0x8000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(18, 2);
    BattleRuntime_WaitIfModeZero_27(20);
    BattleRuntime_RunThenWaitIfModeZero_18(18, 0, 20);
    Object_SetModeById_18(9, 3);
    Object_SetModeById_19(10, 3);
    Object_SetModeById_20(11, 3);
    Object_SetModeById_21(12, 3);
    Object_SetModeById_22(13, 3);
    Object_SetModeById_23(14, 3);
    Object_SetModeById_24(15, 3);
    ObjectMotion_CallThenWaitForAnimationChange_11(16, 3);
    BattleRuntime_WaitIfModeZero_28(20);
    ObjectMotion_ResetAndSetPositionInMode2_3(10, 120, 200);
    ObjectMotion_ResetAndSetPositionInMode2_4(12, 120, 248);
    ObjectMotion_CommitCurrentPositionAndActivate_3(10);
    ObjectMotion_ArmCallback_9(11, 0x8000, 20);
    Object_SetModeById_25(10, 5);
    Object_SetModeById_26(11, 5);
    ObjectMotion_CommitCurrentPositionAndActivate_4(12);
    ObjectMotion_EnableActionAndSetCallback_1(12, 0x200c638);
    ObjectMotion_SetSpeedParameters_6(15, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_4(15, 216, 168);
    ObjectMotion_SetPositionAndReset_5(15, 232, 168);
    ObjectMotion_ArmCallback_10(15, 0xc000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(15, 3);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(19, 0xe80000, 0xa80000);
    actor = Scene_GetRecord_6(19);
    *(s32 *)(actor + 12) = 0xc0000;
    actor = Scene_GetRecord_7(19);
    *(s32 *)(actor + 60) = -0x80000000;
    actor = Scene_GetRecord_8(19);
    {
        s32 target = *(s32 *)(actor + 80);
        s32 shown = 0x8000;

        *(u16 *)(target + 30) = shown;
    }
    Audio_PlayCue_1(124);
    BattleRuntime_WaitIfModeZero_29(40);
    ObjectMotion_SetPositionAndReset_6(15, 216, 152);
    ObjectMotion_ArmCallback_11(15, 0x4000, 30);
    GameFlag_Clear_1(0x898);
    GameFlag_Set_1(0x89b);
}
