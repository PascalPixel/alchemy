#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define NewEffectObject           Func_02000048
#define AcquireOverlayObject      Func_020043e6
#define RunOverlayObjectCommand0  Func_02004424
#define RunOverlayObjectCommand1  Func_0200443c
#define RunOverlayObjectCommand14 Func_0200451c
#define SetEffectMode Func_02000030
#define NewFlippedEffectObject Func_020000a0
#define SpawnEffect Func_0200013c
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define StopXianActor Func_02000314
#define FaceXianActorToPlayer Func_02000324
#define StartSchoolDoorEvent Func_02002484
#define GetXianScriptData Func_0200034c
#define GetXianInitialState Func_0200037c
#define GetXianMessageData Func_02000380
#define Scene_GetRecord_1(args...) Func_020061c6(args)
#define Scene_GetRecord_2(a0) Value1(Func_020061d0, a0)
#define Scene_GetRecord_3(a0) Value1(Func_020061d8, a0)
#define Scene_GetRecord_4(a0) Value1(Func_020061e0, a0)
#define Scene_GetRecord_5(args...) Func_020061e8(args)
#define Object_SetModeById_1(a0, a1) Value2(Func_02006242, a0, a1)
#define Motion_SetSpeedLim_1(a0, a1) Call2(Func_020062de, a0, a1)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_020062f8, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02006304(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020062b2, a0)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02006234, a0, a1, a2)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_0200623e, a0, a1, a2)
#define Motion_ResetPosMode2_1(args...) Func_02006260(args)
#define Motion_SetPosReset_1(args...) Func_02006272(args)
#define Motion_CommitPos_1(args...) Func_02006288(args)
#define Motion_SetAngleToward_1(args...) Func_020062ca(args)
#define Motion_SetAngleToward_2(args...) Func_020062d4(args)
#define Motion_SetAngleToward_3(args...) Func_020062de_a(args)
#define Motion_SetAngleToward_4(args...) Func_020062e8(args)
#define Motion_SetAngleToward_5(args...) Func_020062f2(args)
#define Motion_SetAngleToward_6(args...) Func_020062fc(args)
#define Motion_SetAngleToward_7(args...) Func_02006306(args)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_020062ac, a0, a1, a2)
#define Motion_SetSpeed_4(a0, a1, a2) Call3(Func_020062ba, a0, a1, a2)
#define Motion_ResetPosMode2_2(args...) Func_020062dc(args)
#define Motion_SetPosReset_2(args...) Func_020062ee(args)
#define Motion_SetAngleToward_8(args...) Func_02006340(args)
#define Motion_CommitPos_2(args...) Func_0200630e(args)
#define Motion_SetAngleToward_9(args...) Func_02006350(args)
#define Motion_SetVarCbObj_1(args...) Func_02006350_a(args)
#define Battle_WaitMode0_1(args...) Func_020062b6(args)
#define Battle_RunThenWaitIfModeZero_1(args...) Func_020063a0(args)
#define Motion_SetAngleToward_10(args...) Func_02006372(args)
#define Motion_SetAngleToward_11(args...) Func_0200637c(args)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_020063d0, a0, a1, a2)
#define Motion_SetAngleToward_12(args...) Func_02006392(args)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_020063e6, a0, a1, a2)
#define Motion_SetAngleToward_13(args...) Func_020063a8(args)
#define Motion_SetAngleToward_14(args...) Func_020063b2(args)
#define Motion_SetAngleToward_15(args...) Func_020063bc(args)
#define Battle_WaitMode0_2(args...) Func_0200631a(args)
#define Battle_RunThenWaitIfModeZero_2(args...) Func_02006404(args)
#define Motion_CallWaitAnim_1(args...) Func_020063b4(args)
#define Battle_WaitMode0_3(args...) Func_02006332(args)
#define Motion_CallWaitAnim_2(args...) Func_020063c2(args)
#define Battle_WaitMode0_4(args...) Func_02006340_a(args)
#define Battle_RunThenWaitIfModeZero_3(args...) Func_0200642a(args)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_0200644c, a0, a1, a2)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_02006456, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_4(args...) Func_02006448(args)
#define Battle_RunThenWaitIfModeZero_5(args...) Func_02006452(args)
#define BattleFx_SpawnLinked_3(a0, a1, a2) Call3(Func_02006476, a0, a1, a2)
#define BattleFx_SpawnLinked_4(a0, a1, a2) Call3(Func_02006480, a0, a1, a2)
#define Motion_SetSpeed_5(a0, a1, a2) Call3(Func_020063d2, a0, a1, a2)
#define Motion_SetPosReset_3(args...) Func_020063fc(args)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_02006498, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_6(args...) Func_02006492(args)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_020064ae, a0, a1, a2)
#define Motion_CallWaitAnim_3(args...) Func_0200644e(args)
#define Battle_WaitMode0_5(args...) Func_020063cc(args)
#define Battle_RunThenWaitIfModeZero_7(args...) Func_020064b6(args)
#define Motion_SetVarCb_1(args...) Func_02006476_a(args)
#define Motion_SetVarCb_2(args...) Func_0200647e(args)
#define Motion_SetVarCb_3(args...) Func_02006486(args)
#define Motion_SetVarCb_4(args...) Func_0200648e(args)
#define Motion_SetVarCb_5(args...) Func_02006496(args)
#define Motion_SetVarCb_6(args...) Func_0200649e(args)
#define Motion_SetVarCb_7(args...) Func_020064a6(args)
#define Motion_SetVarCb_8(args...) Func_020064ae_a(args)
#define Battle_WaitMode0_6(args...) Func_0200641c(args)
#define Motion_SetVarCbObj_2(args...) Func_020064c4(args)
#define Battle_RunThenWaitIfModeZero_8(args...) Func_0200650e(args)
#define Motion_CallWaitAnim_4(args...) Func_020064be(args)
#define Battle_WaitMode0_7(args...) Func_0200643c(args)
#define Battle_RunThenWaitIfModeZero_9(args...) Func_02006526(args)
#define Motion_ArmCb_5(a0, a1, a2) Call3(Func_02006542, a0, a1, a2)
#define Motion_ArmCb_6(a0, a1, a2) Call3(Func_0200654e, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_1(a0, a1) Value2(Func_0200654e_a, a0, a1)
#define BattleFx_SpawnLinked_5(a0, a1, a2) Call3(Func_02006568, a0, a1, a2)
#define Battle_WaitMode0_8(args...) Func_02006476_b(args)
#define BattleFx_SpawnLinked_6(a0, a1, a2) Call3(Func_02006578, a0, a1, a2)
#define Battle_WaitMode0_9(args...) Func_02006486_a(args)
#define BattleFx_SpawnLinked_7(a0, a1, a2) Call3(Func_02006588, a0, a1, a2)
#define Battle_WaitMode0_10(args...) Func_02006496_a(args)
#define BattleFx_SpawnLinked_8(a0, a1, a2) Call3(Func_02006598, a0, a1, a2)
#define Battle_WaitMode0_11(args...) Func_020064a6_a(args)
#define BattleFx_SpawnLinked_9(a0, a1, a2) Call3(Func_020065a8, a0, a1, a2)
#define Battle_WaitMode0_12(args...) Func_020064b6_a(args)
#define BattleFx_SpawnLinked_10(a0, a1, a2) Call3(Func_020065b8, a0, a1, a2)
#define Battle_WaitMode0_13(args...) Func_020064c6(args)
#define BattleFx_SpawnLinked_11(a0, a1, a2) Call3(Func_020065c8, a0, a1, a2)
#define Battle_WaitMode0_14(args...) Func_020064d6(args)
#define BattleFx_SpawnLinked_12(a0, a1, a2) Call3(Func_020065d8, a0, a1, a2)
#define Battle_WaitMode0_15(args...) Func_020064e6(args)
#define Motion_SetVarCbObj_3(args...) Func_0200658e(args)
#define Battle_RunThenWaitIfModeZero_10(args...) Func_020065d8_a(args)
#define Motion_CallWaitAnim_5(args...) Func_02006588_a(args)
#define Battle_WaitMode0_16(args...) Func_02006506(args)
#define Battle_RunThenWaitIfModeZero_11(args...) Func_020065f0(args)
#define BattleFx_SpawnLinked_13(a0, a1, a2) Call3(Func_02006612, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_12(args...) Func_02006604(args)
#define Motion_SetAngleToward_16(args...) Func_020065d6(args)
#define Battle_WaitMode0_17(args...) Func_02006534(args)
#define Motion_CallWaitAnim_6(args...) Func_020065c4(args)
#define Battle_WaitMode0_18(args...) Func_02006542_a(args)
#define Battle_RunThenWaitIfModeZero_13(args...) Func_0200662c(args)
#define Object_LinkPair_1(args...) Func_02006606(args)
#define Object_LinkPair_2(args...) Func_02006610(args)
#define Object_LinkPair_3(args...) Func_0200661a(args)
#define Battle_WaitMode0_19(args...) Func_02006570(args)
#define Motion_SetAngleToward_17(args...) Func_02006622(args)
#define Motion_SetAngleToward_18(args...) Func_0200662c_a(args)
#define Motion_SetAngleToward_19(args...) Func_02006636(args)
#define Motion_SetAngleToward_20(args...) Func_02006640(args)
#define Motion_SetAngleToward_21(args...) Func_0200664a(args)
#define Motion_SetAngleToward_22(args...) Func_02006654(args)
#define Battle_WaitMode0_20(args...) Func_020065b2(args)
#define Battle_RunThenWaitIfModeZero_14(args...) Func_0200669c(args)
#define Motion_SetVarCbObj_4(args...) Func_02006664(args)
#define Battle_WaitMode0_21(args...) Func_020065ca(args)
#define Object_SetModeById_2(args...) Func_02006652(args)
#define Object_SetModeById_3(args...) Func_0200665a(args)
#define Object_SetModeById_4(args...) Func_0200667a(args)
#define Object_SetModeById_5(args...) Func_02006682(args)
#define Object_SetModeById_6(args...) Func_0200668a(args)
#define Object_SetModeById_7(args...) Func_02006692(args)
#define Object_SetModeById_8(args...) Func_0200669a(args)
#define Motion_CallWaitAnim_7(args...) Func_020066aa(args)
#define Battle_WaitMode0_22(args...) Func_02006628(args)
#define Motion_ArmCb_7(a0, a1, a2) Call3(Func_02006724, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_2(a0, a1) Value2(Func_02006724_a, a0, a1)
#define Motion_CallWaitAnim_8(args...) Func_020066cc(args)
#define Battle_WaitMode0_23(args...) Func_0200664a_a(args)
#define Battle_RunThenWaitIfModeZero_15(args...) Func_02006734(args)
#define Object_SetModeById_9(args...) Func_020066dc(args)
#define Object_SetModeById_10(args...) Func_020066e4(args)
#define Object_SetModeById_11(args...) Func_020066ec(args)
#define Object_SetModeById_12(args...) Func_020066f4(args)
#define Object_SetModeById_13(args...) Func_020066fc(args)
#define Object_SetModeById_14(args...) Func_02006704(args)
#define Object_SetModeById_15(args...) Func_0200670c(args)
#define Object_SetModeById_16(args...) Func_02006714(args)
#define Motion_CallWaitAnim_9(args...) ((void (*)())Func_02006724_b)(args)
#define Battle_WaitMode0_24(args...) Func_020066a2(args)
#define Motion_SetVarCbObj_5(args...) Func_0200674a(args)
#define Battle_WaitMode0_25(args...) Func_020066b0(args)
#define Battle_RunThenWaitIfModeZero_16(args...) Func_0200679a(args)
#define Battle_RunThenWaitIfModeZero_17(args...) Func_020067a4(args)
#define Object_SetModeById_17(args...) Func_0200674c(args)
#define Motion_CallWaitAnim_10(args...) Func_0200675c(args)
#define Battle_WaitMode0_26(args...) Func_020066da(args)
#define Motion_ArmCb_8(a0, a1, a2) Call3(Func_020067d6, a0, a1, a2)
#define Motion_SetVarCbObj_6(args...) Func_0200678e(args)
#define Battle_WaitMode0_27(args...) Func_020066f4_a(args)
#define Battle_RunThenWaitIfModeZero_18(args...) Func_020067de(args)
#define Object_SetModeById_18(args...) Func_02006786(args)
#define Object_SetModeById_19(args...) Func_0200678e_a(args)
#define Object_SetModeById_20(args...) Func_02006796(args)
#define Object_SetModeById_21(args...) Func_0200679e(args)
#define Object_SetModeById_22(args...) Func_020067a6(args)
#define Object_SetModeById_23(args...) Func_020067ae(args)
#define Object_SetModeById_24(args...) Func_020067b6(args)
#define Motion_CallWaitAnim_11(args...) Func_020067c6(args)
#define Battle_WaitMode0_28(args...) Func_02006744(args)
#define Motion_ResetPosMode2_3(args...) Func_020067a6_a(args)
#define Motion_ResetPosMode2_4(args...) Func_020067b0(args)
#define Motion_CommitPos_3(args...) Func_020067ce(args)
#define Motion_ArmCb_9(a0, a1, a2) Call3(Func_0200685a, a0, a1, a2)
#define Object_SetModeById_25(args...) Func_020067f2(args)
#define Object_SetModeById_26(args...) Func_020067fa(args)
#define Motion_CommitPos_4(args...) Func_020067f0(args)
#define Motion_EnableActCb_1(a0, a1) Value2(Func_020067d0, a0, a1)
#define Motion_SetSpeed_6(a0, a1, a2) Call3(Func_020067d2, a0, a1, a2)
#define Motion_SetPosReset_4(args...) Func_020067fc(args)
#define Motion_SetPosReset_5(args...) Func_02006806(args)
#define Motion_ArmCb_10(a0, a1, a2) Call3(Func_020068a2, a0, a1, a2)
#define Motion_SetVarCbObj_7(args...) Func_0200685a_a(args)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_02006840, a0, a1, a2)
#define Scene_GetRecord_6(args...) Func_020067fe(args)
#define Scene_GetRecord_7(args...) Func_0200680a(args)
#define Scene_GetRecord_8(args...) Func_02006816(args)
#define Audio_PlayCue_1(args...) Func_0200699c(args)
#define Battle_WaitMode0_29(args...) Func_020067fa_a(args)
#define Motion_SetPosReset_6(args...) Func_02006864_a(args)
#define Motion_ArmCb_11(a0, a1, a2) Call3(Func_02006900, a0, a1, a2)
#define GameFlag_Clear_1(a0) Call1(Func_0200680e, a0)
#define GameFlag_Set_1(a0) Call1(Func_0200680c, a0)
#define ACTOR_FIELD_108(rec) (*(s32 *)((rec) + 108))
#define FieldScene_RunScene39e_02000414 Func_02000414
#define Scene_RunEnsembleStoryBeat Func_02001dbc
#define Scene_RunOpeningAuxiliarySequence Func_0200268c
#define Scene_RunScene39eSequenceA Func_020026d8
#define OpenSceneExit Func_02004edc
#define WaitSceneExitStep Func_02004e58
#define ClearSceneExitGateAtEntry Func_02004f0a
#define GetSceneExitPendingWork Func_02004f1e
#define ResetSceneExitPendingWork Func_02004f2a
#define SetSceneExitCompletionMode Func_02004f34
#define ClearSceneExitField40 Func_02004f40
#define SetSceneExitGate Func_02004f48
#define TransitionSceneExitSlot Func_02004fd0
#define IsFlag0895Set Func_02004f16
#define IsFlag089bSet Func_02004f28
#define ShowSceneExitDialogue1a5b Func_02005000
#define ShowSceneExitDialogue189e Func_02005012
#define ShowSceneExitDialogue182a Func_0200502c
#define FinalizeSceneExitSlot Func_02005044
#define SetSceneExitHeading Func_02004fa2
#define ClearSceneExitGateBeforeDescriptor Func_02004fae
#define InstallSceneExitDescriptor Func_02004fd4
#define CloseSceneExit Func_02004fa0
#define AdvanceSceneStep       Func_02005722
#define OpenSceneSection        Func_0200571c
#define CloseSceneSection       Func_02005784
#define GetSceneRecord          Func_0200573e
#define SetSceneActorMode       Func_0200579c
#define SetSceneActorModeAfterBranch Func_020057ea
#define RunFacingVariantA       Func_020021f6
#define RunFacingVariantB       Func_02001f68
#define RunFacingVariantC       Func_0200213c
#define RunFacingVariantD       Func_02002062
#define RunSceneVariant         Func_020023c8
#define Scene_RunFlag88FBranch Func_02000484
#define Scene_ShowDialogue17B1 Func_02000abc
#define Scene_ShowDialogue1825 Func_02000adc
#define Scene_RunRoofSceneExit Func_02000afc
#define Scene_ShowDialogue182D Func_02000bd4
#define Scene_RunForwardArcBurst Func_02000bf4
#define Scene_RunDescentBurst Func_02000db4
#define Scene_DispatchApproachByFacing Func_020012e0
#define Scene_DispatchByFacing Func_02001334
#define Scene_DispatchByFacingAndFlags Func_020013b8
#define Scene_RunSkippableStoryBeat Func_02001d50
#define Scene_ShowDialogue1A58 Func_02002464
#define Scene_DispatchByRange Func_02002508
#define Scene_ShowDialogue17DF Func_0200254c
#define Scene_SelectData Func_02002574
#define Scene_SpawnRandomizedParticle Func_020025b8
#define Scene_ApplyOffset0Neg32 Func_0200265c
#define Scene_ApplyOffset0Pos32 Func_0200266c
#define Scene_ApplyOffsetNeg32_0 Func_0200267c
#define Scene_PlaySound123AndEnable Func_02002764
#define Scene_SetFlag140AndFinishSequence Func_020040dc
#define Scene_FinishSequence Func_02004128
#define Scene_SpawnEightShots Func_02004140
#define Scene_SelectActorModeFromInputBit Func_020041c4
#define Scene_RunParticleRain Func_020041ec
#define Scene_RunSecondEnsembleBeat Func_02001494
#define Func_08009178 Func_02005934
#define Func_080770c8 Func_0200585e
#define Func_0808a010 Func_02005874
#define Func_0808a070 Func_02006026
#define Func_0808a080 Func_020059c6
#define Func_0808a088 Func_02005e26
#define Func_0808a090 Func_02005976
#define Func_0808a098 Func_0200615c
#define Func_0808a0c8 Func_02005b04
#define Func_0808a0d0 Func_020059a0
#define Func_0808a0e8 Func_02005b30
#define Func_0808a0f0 Func_02005e60
#define Func_0808a100 Func_02006184
#define Func_0808a110 Func_02005c54
#define Func_0808a130 Func_02005ae8
#define Func_0808a138 Func_02005a1c
#define Func_0808a148 Func_02005926
#define Func_0808a170 Func_02005990
#define Func_0808a178 Func_020060ce
#define Func_0808a188 Func_020059b2
#define Func_0808a190 Func_02005a06
#define Func_0808a1b8 Func_02005c94
#define Func_0808a1e8 Func_0200599c
#define Func_0808a200 Func_02005a34
#define Func_080f9010 Func_02005c80
#define Battle_Reset_1(args...) Func_02006eb0(args)
#define ObjectMotion_SetSpeedParameters_1_02002778(a0, a1, a2) Call3(Func_02006ef2, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02002778(a0, a1, a2) Call3(Func_02006f18, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1_02002778(args...) Func_02007014(args)
#define ObjectMotion_SetSpeedParameters_2_02002778(args...) Func_02007028(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_02002778(args...) Func_02006f3e(args)
#define ObjectMotion_ArmCallback_1_02002778(args...) Func_02006fc8(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02002778(args...) Func_02006f80(args)
#define BattleRuntime_WaitIfModeZero_2_02002778(args...) Func_02006ee6(args)
#define SceneWork_SetStepValue_1_02002778(a0) Call1(Func_02006fb4, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1_02002778(args...) Func_02006fd6(args)
#define ObjectMotion_ArmCallback_2_02002778(a0, a1, a2) Call3(Func_02006ff2, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_1_02002778(args...) Func_02006f92(args)
#define BattleRuntime_WaitIfModeZero_3_02002778(args...) Func_02006f10(args)
#define ObjectMotion_ArmCallback_3_02002778(a0, a1, a2) Call3(Func_0200700c, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_2_02002778(args...) Func_02006fac(args)
#define BattleRuntime_WaitIfModeZero_4_02002778(args...) Func_02006f2a(args)
#define BattleRuntime_RunThenWaitIfModeZero_2_02002778(args...) Func_02007014_a(args)
#define ObjectMotion_ArmCallback_4_02002778(a0, a1, a2) Call3(Func_02007030, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_02002778(args...) Func_02006fe8(args)
#define BattleRuntime_WaitIfModeZero_5_02002778(args...) Func_02006f4e(args)
#define ObjectMotion_ArmCallback_5_02002778(a0, a1, a2) Call3(Func_0200704a, a0, a1, a2)
#define Scene_GetRecord_1_02002778(args...) Func_02006f90(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02002778(args...) Func_02006fe6(args)
#define Scene_GetRecord_2_02002778(args...) Func_02006fa4(args)
#define Motion_SetHPosTerrain_2(args...) Func_02006ffa(args)
#define Scene_GetRecord_3_02002778(args...) Func_02006fb8(args)
#define Motion_SetHPosTerrain_3(args...) Func_0200700e(args)
#define ObjectMotion_SetSpeedParameters_3_02002778(a0, a1, a2) Call3(Func_02006fe4, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4_02002778(a0, a1, a2) Call3(Func_02006ff2_a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5_02002778(a0, a1, a2) Call3(Func_02007000, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6_02002778(a0, a1, a2) Call3(Func_0200700e_a, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_2_02002778(a0, a1, a2) Call3(Func_02007034, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_3_02002778(a0, a1, a2) Call3(Func_02007042, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_4_02002778(a0, a1, a2) Call3(Func_02007050, a0, a1, a2)
#define Motion_ResetPosMode2_5(a0, a1, a2) Call3(Func_0200705e, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2_02002778(args...) Func_0200707c(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_3_02002778(args...) Func_02007082(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_4_02002778(args...) Func_02007088(args)
#define ObjectMotion_ArmCallback_6_02002778(a0, a1, a2) Call3(Func_02007114, a0, a1, a2)
#define Motion_CommitPos_5(args...) Func_0200709a(args)
#define ObjectMotion_ArmCallback_7_02002778(a0, a1, a2) Call3(Func_02007126, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_6_02002778(args...) Func_0200703c(args)
#define BattleEffect_SpawnLinkedResourceObject_1_02002778(a0, a1, a2) Call3(Func_0200713e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2_02002778(a0, a1, a2) Call3(Func_02007148, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3_02002778(a0, a1, a2) Call3(Func_02007152, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4_02002778(a0, a1, a2) Call3(Func_0200715c, a0, a1, a2)
#define ObjectMotion_ArmCallback_8_02002778(a0, a1, a2) Call3(Func_02007160, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_1_02002778(args...) Func_02007160_a(args)
#define BattleRuntime_WaitIfModeZero_7_02002778(args...) Func_0200707e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3_02002778(args...) Func_0200710e(args)
#define BattleRuntime_WaitIfModeZero_8_02002778(args...) Func_0200708c(args)
#define BattleRuntime_RunThenWaitIfModeZero_3_02002778(args...) Func_02007176(args)
#define ObjectMotion_ArmCallback_9_02002778(a0, a1, a2) Call3(Func_02007192, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_5_02002778(a0, a1, a2) Call3(Func_020071a6, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_4_02002778(args...) Func_02007198(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4_02002778(args...) Func_02007148_a(args)
#define BattleRuntime_WaitIfModeZero_9_02002778(args...) Func_020070c6(args)
#define BattleRuntime_RunThenWaitIfModeZero_5_02002778(args...) Func_020071b0(args)
#define BattleEffect_SpawnLinkedResourceObject_6_02002778(a0, a1, a2) Call3(Func_020071d4, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_6_02002778(args...) Func_020071c6(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5_02002778(args...) Func_02007176_a(args)
#define ObjectMotion_ArmCallback_10_02002778(a0, a1, a2) Call3(Func_020071ea, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_7_02002778(args...) Func_020071e4(args)
#define ObjectMotion_ArmCallback_11_02002778(args...) Func_020071fe(args)
#define Motion_ArmCb_12(a0, a1, a2) Call3(Func_0200720a, a0, a1, a2)
#define Motion_ArmCb_13(a0, a1, a2) Call3(Func_02007216, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1_02002778(args...) Func_020071c6_a(args)
#define BattleRuntime_WaitIfModeZero_10_02002778(a0, a1) Call2(Func_02007238, a0, a1)
#define BattleRuntime_WaitIfModeZero_11_02002778(args...) Func_0200713e_a(args)
#define Motion_ArmCb_14(a0, a1, a2) Call3(Func_0200723a, a0, a1, a2)
#define Motion_ArmCb_15(a0, a1, a2) Call3(Func_02007246, a0, a1, a2)
#define Motion_ArmCb_16(a0, a1, a2) Call3(Func_02007252, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_12_02002778(args...) Func_02007168(args)
#define BattleRuntime_RunThenWaitIfModeZero_8_02002778(args...) Func_02007252_a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6_02002778(args...) Func_02007202(args)
#define BattleRuntime_WaitIfModeZero_13_02002778(args...) Func_02007180(args)
#define BattleRuntime_RunThenWaitIfModeZero_9_02002778(args...) Func_0200726a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3_02002778(args...) Func_02007232(args)
#define BattleRuntime_WaitIfModeZero_14_02002778(args...) Func_02007198_a(args)
#define BattleRuntime_RunThenWaitIfModeZero_10_02002778(args...) Func_02007282(args)
#define BattleRuntime_RunThenWaitIfModeZero_11_02002778(args...) Func_0200728c(args)
#define Object_LinkPair_1_02002778(args...) Func_02007266(args)
#define Motion_ArmCb_17(a0, a1, a2) Call3(Func_020072b2, a0, a1, a2)
#define Motion_ArmCb_18(a0, a1, a2) Call3(Func_020072be, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_7_02002778(args...) Func_0200725e(args)
#define BattleRuntime_WaitIfModeZero_15_02002778(args...) Func_020071dc(args)
#define BattleRuntime_RunThenWaitIfModeZero_12_02002778(args...) Func_020072c6(args)
#define BattleEffect_SpawnLinkedResourceObject_7_02002778(a0, a1, a2) Call3(Func_020072e8, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_13_02002778(args...) Func_020072da(args)
#define Motion_ArmCb_19(a0, a1, a2) Call3(Func_020072f6, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_14_02002778(args...) Func_020072f0(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4_02002778(args...) Func_020072b8(args)
#define BattleRuntime_WaitIfModeZero_16_02002778(args...) Func_0200721e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_8_02002778(args...) Func_020072ae(args)
#define BattleRuntime_WaitIfModeZero_17_02002778(args...) Func_0200722c(args)
#define BattleRuntime_RunThenWaitIfModeZero_15_02002778(args...) Func_02007316(args)
#define Motion_ArmCb_20(a0, a1, a2) Call3(Func_02007332, a0, a1, a2)
#define Motion_ArmCb_21(a0, a1, a2) Call3(Func_0200733e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_18_02002778(args...) Func_02007254(args)
#define Motion_ArmCb_22(a0, a1, a2) Call3(Func_02007350, a0, a1, a2)
#define Motion_ArmCb_23(a0, a1, a2) Call3(Func_0200735c, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_16_02002778(args...) Func_02007356(args)
#define ObjectMotion_CallThenWaitForAnimationChange_9_02002778(args...) Func_02007306(args)
#define BattleRuntime_WaitIfModeZero_19_02002778(args...) Func_02007284(args)
#define ObjectMotion_SetAngleToward_1_02002778(args...) Func_02007336(args)
#define BattleRuntime_WaitIfModeZero_20_02002778(args...) Func_02007294(args)
#define BattleRuntime_RunThenWaitIfModeZero_17_02002778(args...) Func_02007392(args)
#define ObjectMotion_SetAngleToward_2_02002778(args...) Func_02007364(args)
#define ObjectMotion_SetAngleToward_3_02002778(args...) Func_0200736e(args)
#define ObjectMotion_SetAngleToward_4_02002778(args...) Func_02007378(args)
#define BattleEffect_SpawnLinkedResourceObject_8_02002778(a0, a1, a2) Call3(Func_020073d4, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_18_02002778(args...) Func_020073c6(args)
#define ObjectMotion_CallThenWaitForAnimationChange_10_02002778(args...) Func_02007376(args)
#define BattleRuntime_WaitIfModeZero_21_02002778(args...) Func_020072f4(args)
#define Battle_RunThenWaitIfModeZero_19(args...) Func_020073de(args)
#define ObjectMotion_CallThenWaitForAnimationChange_11_02002778(args...) Func_0200738e(args)
#define BattleRuntime_WaitIfModeZero_22_02002778(args...) Func_0200730c(args)
#define ObjectMotion_SetVariantCallback_2_02002778(args...) Func_020073ac(args)
#define BattleRuntime_WaitIfModeZero_23_02002778(args...) Func_0200731a(args)
#define Battle_RunThenWaitIfModeZero_20(args...) Func_02007404(args)
#define BattleEffect_SpawnLinkedResourceObject_9_02002778(a0, a1, a2) Call3(Func_02007428, a0, a1, a2)
#define ObjectMotion_SetAngleToward_5_02002778(args...) Func_020073e2(args)
#define BattleRuntime_WaitIfModeZero_24_02002778(args...) Func_02007340(args)
#define Battle_RunThenWaitIfModeZero_21(args...) Func_0200742a(args)
#define BattleEffect_SpawnLinkedResourceObject_10_02002778(a0, a1, a2) Call3(Func_0200744e, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_22(args...) Func_02007440(args)
#define ObjectMotion_SetAngleToward_6_02002778(args...) Func_02007412(args)
#define Motion_ArmCb_24(a0, a1, a2) Call3(Func_02007466, a0, a1, a2)
#define Motion_ArmCb_25(a0, a1, a2) Call3(Func_02007472, a0, a1, a2)
#define Motion_ArmCb_26(a0, a1, a2) Call3(Func_0200747e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_25_02002778(args...) Func_02007394(args)
#define Motion_CallWaitAnim_12(args...) Func_02007424(args)
#define BattleRuntime_WaitIfModeZero_26_02002778(args...) Func_020073a2(args)
#define Battle_RunThenWaitIfModeZero_23(args...) Func_0200748c(args)
#define ObjectMotion_SetVariantCallback_3_02002778(args...) Func_0200744c(args)
#define BattleRuntime_WaitIfModeZero_27_02002778(args...) Func_020073ba(args)
#define Battle_RunThenWaitIfModeZero_24(args...) Func_020074a4(args)
#define Motion_CallWaitAnim_13(args...) Func_02007454(args)
#define BattleRuntime_WaitIfModeZero_28_02002778(args...) Func_020073d2(args)
#define Battle_RunThenWaitIfModeZero_25(args...) Func_020074bc(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5_02002778(args...) Func_02007484(args)
#define BattleRuntime_WaitIfModeZero_29_02002778(args...) Func_020073ea(args)
#define Battle_RunThenWaitIfModeZero_26(args...) Func_020074d4(args)
#define Motion_ArmCb_27(a0, a1, a2) Call3(Func_020074f0, a0, a1, a2)
#define Motion_CallWaitAnim_14(args...) Func_02007490(args)
#define Battle_WaitMode0_30(args...) Func_0200740e(args)
#define Battle_RunThenWaitIfModeZero_27(args...) Func_020074f8(args)
#define ObjectMotion_SetVariantCallback_4_02002778(args...) Func_020074b8(args)
#define ObjectMotion_SetVariantCallback_5_02002778(args...) Func_020074c0(args)
#define ObjectMotion_SetVariantCallback_6_02002778(args...) Func_020074c8(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6_02002778(args...) Func_020074d8(args)
#define Battle_WaitMode0_31(args...) Func_0200743e(args)
#define Motion_CallWaitAnim_15(args...) Func_020074ce(args)
#define Battle_WaitMode0_32(args...) Func_0200744c_a(args)
#define Battle_RunThenWaitIfModeZero_28(args...) Func_02007536(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7_02002778(args...) Func_020074fe(args)
#define Battle_WaitMode0_33(args...) Func_02007464(args)
#define Battle_RunThenWaitIfModeZero_29(args...) Func_0200754e(args)
#define Object_SetModeById_1_02002778(args...) Func_020074f6(args)
#define Object_SetModeById_2_02002778(args...) Func_020074fe_a(args)
#define Object_SetModeById_3_02002778(args...) Func_02007506(args)
#define Motion_CallWaitAnim_16(args...) Func_02007516(args)
#define Battle_WaitMode0_34(args...) Func_02007494(args)
#define Battle_RunThenWaitIfModeZero_30(args...) Func_0200757e(args)
#define BattleEffect_SpawnLinkedResourceObject_11_02002778(a0, a1, a2) Call3(Func_020075a0, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_7_02002778(args...) Func_02007548(args)
#define Battle_WaitMode0_35(args...) Func_020074b6(args)
#define Battle_RunThenWaitIfModeZero_31(args...) Func_020075a0_a(args)
#define Motion_CallWaitAnim_17(args...) Func_02007550(args)
#define Battle_WaitMode0_36(args...) Func_020074ce_a(args)
#define Battle_RunThenWaitIfModeZero_32(args...) Func_020075b8(args)
#define BattleEffect_SpawnLinkedResourceObject_12_02002778(a0, a1, a2) Call3(Func_020075da, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_13_02002778(a0, a1, a2) Call3(Func_020075e4, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_33(args...) Func_020075d6(args)
#define BattleFx_SpawnLinked_14(a0, a1, a2) Call3(Func_020075fa, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_8_02002778(args...) Func_020075a2(args)
#define Battle_WaitMode0_37(args...) Func_02007510(args)
#define Motion_CallWaitAnim_18(args...) Func_020075a0_b(args)
#define Battle_WaitMode0_38(args...) Func_0200751e(args)
#define Battle_RunThenWaitIfModeZero_34(args...) Func_02007608(args)
#define Battle_RunThenWaitIfModeZero_35(args...) Func_02007612(args)
#define Motion_SetVarCb_9(args...) Func_020075d2(args)
#define Motion_SetVarCb_10(args...) Func_020075da_a(args)
#define Motion_SetVarCb_11(args...) Func_020075e2(args)
#define Motion_SetVarCbObj_8(args...) Func_020075f2(args)
#define Battle_WaitMode0_39(args...) Func_02007558(args)
#define Motion_CallWaitAnim_19(args...) Func_020075e8(args)
#define Battle_WaitMode0_40(args...) Func_02007566(args)
#define Battle_RunThenWaitIfModeZero_36(args...) Func_02007650(args)
#define BattleFx_SpawnLinked_15(a0, a1, a2) Call3(Func_02007672, a0, a1, a2)
#define Motion_ArmCb_28(a0, a1, a2) Call3(Func_02007676, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_2_02002778(args...) Func_02007676_a(args)
#define Battle_WaitMode0_41(args...) Func_02007594(args)
#define Motion_SetVarCbObj_9(args...) Func_0200763c(args)
#define Battle_WaitMode0_42(args...) Func_020075a2_a(args)
#define Battle_RunThenWaitIfModeZero_37(args...) Func_0200768c(args)
#define Motion_ArmCb_29(a0, a1, a2) Call3(Func_020076a8, a0, a1, a2)
#define Motion_ArmCb_30(a0, a1, a2) Call3(Func_020076b4, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_38(args...) Func_020076ae(args)
#define Object_SetModeById_4_02002778(args...) Func_02007656(args)
#define Object_SetModeById_5_02002778(args...) Func_0200765e(args)
#define Object_SetModeById_6_02002778(args...) Func_02007666(args)
#define Motion_CallWaitAnim_20(args...) Func_02007676_b(args)
#define Battle_WaitMode0_43(args...) Func_020075f4(args)
#define Motion_ArmCb_31(a0, a1, a2) Call3(Func_020076f0, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_39(args...) Func_020076ea(args)
#define Motion_SetVarCb_12(args...) Func_020076aa(args)
#define Motion_SetVarCb_13(args...) Func_020076b2(args)
#define Motion_SetVarCb_14(args...) Func_020076ba(args)
#define Motion_SetVarCbObj_10(args...) Func_020076ca(args)
#define Battle_WaitMode0_44(args...) Func_02007630(args)
#define Motion_CallWaitAnim_21(args...) Func_020076c0(args)
#define Battle_WaitMode0_45(args...) Func_0200763e(args)
#define Battle_RunThenWaitIfModeZero_40(args...) Func_02007728(args)
#define BattleFx_SpawnLinked_16(a0, a1, a2) Call3(Func_0200774a, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_41(args...) Func_0200773c(args)
#define Motion_CallWaitAnim_22(args...) Func_020076ec(args)
#define Battle_WaitMode0_46(args...) Func_0200766a(args)
#define Battle_RunThenWaitIfModeZero_42(args...) Func_02007754(args)
#define Battle_RunThenWaitIfModeZero_43(args...) Func_0200775e(args)
#define Battle_WaitMode0_47(a0, a1) Call2(Func_02007788, a0, a1)
#define Battle_WaitMode0_48(a0, a1) Call2(Func_02007792, a0, a1)
#define Battle_WaitMode0_49(a0, a1) Call2(Func_0200779c, a0, a1)
#define Battle_WaitMode0_50(a0, a1) Call2(Func_020077a6, a0, a1)
#define Battle_WaitMode0_51(args...) Func_020076ac(args)
#define Battle_RunThenWaitIfModeZero_44(args...) Func_02007796(args)
#define Motion_ArmCb_32(a0, a1, a2) Call3(Func_020077b2, a0, a1, a2)
#define Battle_WaitMode0_52(args...) Func_020076c8(args)
#define Battle_RunThenWaitIfModeZero_45(args...) Func_020077b2_a(args)
#define BattleFx_SpawnLinked_17(a0, a1, a2) Call3(Func_020077d4, a0, a1, a2)
#define BattleFx_SpawnLinked_18(a0, a1, a2) Call3(Func_020077de, a0, a1, a2)
#define BattleFx_SpawnLinked_19(a0, a1, a2) Call3(Func_020077e8, a0, a1, a2)
#define BattleFx_SpawnLinked_20(a0, a1, a2) Call3(Func_020077f2, a0, a1, a2)
#define Motion_SetVarCbObj_11(args...) Func_020077a2(args)
#define Battle_WaitMode0_53(args...) Func_02007708(args)
#define Battle_RunThenWaitIfModeZero_46(args...) Func_020077f2_a(args)
#define ObjectMotion_SetAngleToward_7_02002778(args...) Func_020077c4(args)
#define ObjectMotion_SetAngleToward_8_02002778(args...) Func_020077ce(args)
#define ObjectMotion_SetAngleToward_9_02002778(args...) Func_020077d8(args)
#define Battle_WaitMode0_54(args...) Func_02007736(args)
#define Object_SetModeById_7_02002778(args...) Func_020077be(args)
#define Object_SetModeById_8_02002778(args...) Func_020077c6(args)
#define Motion_CallWaitAnim_23(args...) Func_020077d6(args)
#define Battle_WaitMode0_55(args...) Func_02007754_a(args)
#define Motion_ArmCb_33(a0, a1, a2) Call3(Func_02007850, a0, a1, a2)
#define Motion_ArmCb_34(a0, a1, a2) Call3(Func_0200785c, a0, a1, a2)
#define Motion_ArmCb_35(a0, a1, a2) Call3(Func_02007868, a0, a1, a2)
#define Battle_WaitMode0_56(args...) Func_0200777e(args)
#define BattleEventRuntime_ProcessAction_3(a0, a1) Value2(Func_0200786e, a0, a1)
#define BattleFx_SpawnLinked_21(a0, a1, a2) Call3(Func_0200788a, a0, a1, a2)
#define BattleFx_SpawnLinked_22(a0, a1, a2) Call3(Func_02007896, a0, a1, a2)
#define BattleFx_SpawnLinked_23(a0, a1, a2) Call3(Func_020078a2, a0, a1, a2)
#define BattleFx_SpawnLinked_24(a0, a1, a2) Call3(Func_020078ae, a0, a1, a2)
#define Battle_WaitMode0_57(args...) Func_020077bc(args)
#define Motion_ArmCb_36(a0, a1, a2) Call3(Func_020078b8, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_4(args...) Func_020078b8_a(args)
#define Battle_WaitMode0_58(args...) Func_020077d6_a(args)
#define Motion_ArmCb_37(a0, a1, a2) Call3(Func_020078d2, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_47(args...) Func_020078cc(args)
#define Motion_ArmCb_38(a0, a1, a2) Call3(Func_020078e8, a0, a1, a2)
#define Motion_ArmCb_39(a0, a1, a2) Call3(Func_020078f4, a0, a1, a2)
#define Motion_ArmCb_40(args...) Func_020078fe(args)
#define BattleFx_SpawnLinked_25(a0, a1, a2) Call3(Func_02007910, a0, a1, a2)
#define BattleFx_SpawnLinked_26(a0, a1, a2) Call3(Func_0200791a, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_48(args...) Func_0200790c(args)
#define Motion_ArmCb_41(a0, a1, a2) Call3(Func_02007928, a0, a1, a2)
#define BattleFx_SpawnLinked_27(a0, a1, a2) Call3(Func_0200793a, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_49(args...) Func_0200792c(args)
#define BattleFx_SpawnLinked_28(a0, a1, a2) Call3(Func_0200794e, a0, a1, a2)
#define BattleFx_SpawnLinked_29(a0, a1, a2) Call3(Func_02007958, a0, a1, a2)
#define BattleFx_SpawnLinked_30(a0, a1, a2) Call3(Func_02007962, a0, a1, a2)
#define BattleFx_SpawnLinked_31(a0, a1, a2) Call3(Func_0200796c, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_50(args...) Func_0200795e(args)
#define BattleFx_SpawnLinked_32(a0, a1, a2) Call3(Func_02007980, a0, a1, a2)
#define BattleFx_SpawnLinked_33(a0, a1, a2) Call3(Func_0200798a, a0, a1, a2)
#define BattleFx_SpawnLinked_34(a0, a1, a2) Call3(Func_02007994, a0, a1, a2)
#define BattleFx_SpawnLinked_35(a0, a1, a2) Call3(Func_020079ac, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_51(args...) Func_0200799e(args)
#define Object_SetModeById_9_02002778(args...) Func_02007946(args)
#define Object_SetModeById_10_02002778(args...) Func_0200794e_a(args)
#define Object_SetModeById_11_02002778(args...) Func_02007956(args)
#define Motion_CallWaitAnim_24(args...) Func_02007966(args)
#define Battle_WaitMode0_59(args...) Func_020078e4(args)
#define Motion_SetVarCbObj_12(args...) Func_0200798c(args)
#define Battle_WaitMode0_60(args...) Func_020078f2(args)
#define Battle_RunThenWaitIfModeZero_52(args...) Func_020079dc(args)
#define Motion_ArmCb_42(args...) Func_020079f6(args)
#define Motion_ArmCb_43(a0, a1, a2) Call3(Func_02007a02, a0, a1, a2)
#define Motion_ArmCb_44(a0, a1, a2) Call3(Func_02007a0e, a0, a1, a2)
#define Motion_ArmCb_45(a0, a1, a2) Call3(Func_02007a1a, a0, a1, a2)
#define Battle_WaitMode0_61(args...) Func_02007930(args)
#define Motion_ArmCb_46(a0, a1, a2) Call3(Func_02007a2c, a0, a1, a2)
#define Motion_ArmCb_47(a0, a1, a2) Call3(Func_02007a38, a0, a1, a2)
#define Motion_ArmCb_48(a0, a1, a2) Call3(Func_02007a44, a0, a1, a2)
#define Motion_ArmCb_49(a0, a1, a2) Call3(Func_02007a50, a0, a1, a2)
#define Battle_WaitMode0_62(args...) Func_02007966_a(args)
#define Motion_ArmCb_50(a0, a1, a2) Call3(Func_02007a62, a0, a1, a2)
#define Motion_CallWaitAnim_25(args...) Func_02007a02_a(args)
#define Battle_WaitMode0_63(args...) Func_02007980_a(args)
#define Battle_RunThenWaitIfModeZero_53(args...) Func_02007a6a(args)
#define Motion_SetHPosTerrain_4(a0, a1, a2) Call3(Func_02007a10, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_54(args...) Func_02007a82(args)
#define BattleFx_SpawnLinked_36(a0, a1, a2) Call3(Func_02007aa6, a0, a1, a2)
#define BattleFx_SpawnLinked_37(a0, a1, a2) Call3(Func_02007ab2, a0, a1, a2)
#define BattleFx_SpawnLinked_38(a0, a1, a2) Call3(Func_02007abe, a0, a1, a2)
#define BattleFx_SpawnLinked_39(a0, a1, a2) Call3(Func_02007aca, a0, a1, a2)
#define BattleFx_SpawnLinked_40(a0, a1, a2) Call3(Func_02007ad6, a0, a1, a2)
#define BattleFx_SpawnLinked_41(a0, a1, a2) Call3(Func_02007ae2, a0, a1, a2)
#define Battle_WaitMode0_64(args...) Func_020079f0(args)
#define Motion_ArmCb_51(a0, a1, a2) Call3(Func_02007aec, a0, a1, a2)
#define Motion_ArmCb_52(a0, a1, a2) Call3(Func_02007af8, a0, a1, a2)
#define Motion_ArmCb_53(a0, a1, a2) Call3(Func_02007b04, a0, a1, a2)
#define Motion_ArmCb_54(a0, a1, a2) Call3(Func_02007b10, a0, a1, a2)
#define Motion_ArmCb_55(args...) Func_02007b1a(args)
#define Battle_WaitMode0_65(args...) Func_02007a30(args)
#define Motion_SetSpeed_7(a0, a1, a2) Call3(Func_02007a7a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_02002778(a0, a1, a2) Call3(Func_02007aa8, a0, a1, a2)
#define BattleFx_SpawnLinked_42(a0, a1, a2) Call3(Func_02007b4a, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_55(args...) Func_02007b3c(args)
#define Motion_ArmCb_56(a0, a1, a2) Call3(Func_02007b58, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2_02002778(a0, a1, a2) Call3(Func_02007ad6_a, a0, a1, a2)
#define Battle_WaitMode0_66(args...) Func_02007a7c(args)
#define Motion_ArmCb_57(a0, a1, a2) Call3(Func_02007b78, a0, a1, a2)
#define Motion_ArmCb_58(args...) Func_02007b82(args)
#define ObjectMotion_SetPositionAndReset_3_02002778(a0, a1, a2) Call3(Func_02007b00, a0, a1, a2)
#define Battle_WaitMode0_67(args...) Func_02007aa6_a(args)
#define Motion_ArmCb_59(a0, a1, a2) Call3(Func_02007ba2, a0, a1, a2)
#define Motion_SetVarCbObj_13(args...) Func_02007b5a(args)
#define Battle_WaitMode0_68(args...) Func_02007ac0(args)
#define Battle_RunThenWaitIfModeZero_56(args...) Func_02007baa(args)
#define BattleFx_SpawnLinked_43(a0, a1, a2) Call3(Func_02007bcc, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_57(args...) Func_02007bbe(args)
#define Motion_SetVarCb_15(args...) Func_02007b7e(args)
#define Battle_WaitMode0_69(a0, a1) Call2(Func_02007bf0, a0, a1)
#define Battle_WaitMode0_70(args...) Func_02007af6(args)
#define Battle_RunThenWaitIfModeZero_58(args...) Func_02007be0(args)
#define BattleFx_SpawnLinked_44(a0, a1, a2) Call3(Func_02007c02, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_59(args...) Func_02007bf4(args)
#define BattleFx_SpawnLinked_45(a0, a1, a2) Call3(Func_02007c18, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_60(args...) Func_02007c0a(args)
#define Motion_SetVarCb_16(args...) Func_02007bca(args)
#define BattleFx_SpawnLinked_46(a0, a1, a2) Call3(Func_02007c36, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_61(args...) Func_02007c28(args)
#define Motion_ArmCb_60(a0, a1, a2) Call3(Func_02007c44, a0, a1, a2)
#define Motion_ArmCb_61(a0, a1, a2) Call3(Func_02007c50, a0, a1, a2)
#define Motion_ArmCb_62(a0, a1, a2) Call3(Func_02007c5c, a0, a1, a2)
#define Motion_ArmCb_63(a0, a1, a2) Call3(Func_02007c68, a0, a1, a2)
#define Battle_WaitMode0_71(args...) Func_02007b7e_a(args)
#define Motion_CallWaitAnim_26(args...) Func_02007c0e(args)
#define Battle_WaitMode0_72(args...) Func_02007b8c(args)
#define Battle_RunThenWaitIfModeZero_62(args...) Func_02007c76(args)
#define Motion_CallWaitAnim_27(args...) Func_02007c26(args)
#define Battle_WaitMode0_73(args...) Func_02007ba4(args)
#define Battle_RunThenWaitIfModeZero_63(args...) Func_02007c8e(args)
#define BattleFx_SpawnLinked_47(a0, a1, a2) Call3(Func_02007cb0, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_64(args...) Func_02007ca2(args)
#define Motion_SetVarCb_17(args...) Func_02007c62(args)
#define Battle_WaitMode0_74(args...) Func_02007bd0(args)
#define Battle_RunThenWaitIfModeZero_65(args...) Func_02007cba(args)
#define BattleFx_SpawnLinked_48(a0, a1, a2) Call3(Func_02007cdc, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_66(args...) Func_02007cce(args)
#define BattleFx_SpawnLinked_49(a0, a1, a2) Call3(Func_02007cf2, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_67(args...) Func_02007ce4(args)
#define Object_SetModeById_12_02002778(args...) Func_02007c8c(args)
#define Battle_WaitMode0_75(args...) Func_02007c12(args)
#define Object_SetModeById_13_02002778(args...) Func_02007c9a(args)
#define Battle_WaitMode0_76(args...) Func_02007c20(args)
#define Object_SetModeById_14_02002778(args...) Func_02007ca8(args)
#define Battle_WaitMode0_77(args...) Func_02007c2e(args)
#define Motion_CallWaitAnim_28(args...) Func_02007cbe(args)
#define ObjectMotion_SetAngleToward_10_02002778(args...) Func_02007ce8(args)
#define Battle_WaitMode0_78(args...) Func_02007c46(args)
#define Battle_RunThenWaitIfModeZero_68(args...) Func_02007d30(args)
#define Motion_SetSpeed_8(a0, a1, a2) Call3(Func_02007c9a_a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4_02002778(a0, a1, a2) Call3(Func_02007cc8, a0, a1, a2)
#define Battle_WaitMode0_79(args...) Func_02007c6e(args)
#define Motion_SetVarCb_18(args...) Func_02007d0e(args)
#define Battle_WaitMode0_80(args...) Func_02007c7c(args)
#define Battle_RunThenWaitIfModeZero_69(args...) Func_02007d66(args)
#define ObjectMotion_SetAngleToward_11_02002778(args...) Func_02007d38(args)
#define Motion_SetSpeed_9(a0, a1, a2) Call3(Func_02007cda, a0, a1, a2)
#define Motion_ArmCb_64(a0, a1, a2) Call3(Func_02007d96, a0, a1, a2)
#define Motion_ArmCb_65(a0, a1, a2) Call3(Func_02007db6, a0, a1, a2)
#define Motion_ArmCb_66(a0, a1, a2) Call3(Func_02007dc2, a0, a1, a2)
#define Motion_ArmCb_67(a0, a1, a2) Call3(Func_02007dce, a0, a1, a2)
#define Motion_CommitPos_6(args...) Func_02007d54(args)
#define Battle_WaitMode0_81(args...) Func_02007cea(args)
#define Motion_CallWaitAnim_29(args...) Func_02007d7a(args)
#define Battle_WaitMode0_82(args...) Func_02007cf8(args)
#define Battle_RunThenWaitIfModeZero_70(args...) Func_02007de2(args)
#define Motion_ResetPosMode2_6(a0, a1, a2) Call3(Func_02007d68, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_5_02002778(a0, a1, a2) Call3(Func_02007d7e, a0, a1, a2)
#define Motion_SetVarCbObj_14(args...) Func_02007dc6(args)
#define Motion_CommitPos_7(args...) Func_02007d9c(args)
#define Battle_RunThenWaitIfModeZero_71(args...) Func_02007e16(args)
#define Motion_SetVarCb_19(args...) Func_02007dd6(args)
#define BattleFx_SpawnLinked_50(a0, a1, a2) Call3(Func_02007e42, a0, a1, a2)
#define Motion_ArmCb_68(a0, a1, a2) Call3(Func_02007e52, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_72(args...) Func_02007e4c(args)
#define Motion_SetVarCb_20(args...) Func_02007e0c(args)
#define Motion_SetVarCb_21(args...) Func_02007e14(args)
#define Motion_SetVarCb_22(args...) Func_02007e1c(args)
#define Motion_SetVarCbObj_15(args...) Func_02007e2c(args)
#define ObjectMotion_SetPositionAndReset_6_02002778(a0, a1, a2) Call3(Func_02007dfa, a0, a1, a2)
#define Motion_ArmCb_69(args...) Func_02007e94(args)
#define Motion_ArmCb_70(args...) Func_02007e9e(args)
#define Motion_ArmCb_71(a0, a1, a2) Call3(Func_02007eaa, a0, a1, a2)
#define Motion_ArmCb_72(a0, a1, a2) Call3(Func_02007eb6, a0, a1, a2)
#define Motion_CallWaitAnim_30(args...) Func_02007e56(args)
#define Battle_WaitMode0_83(args...) Func_02007dd4(args)
#define Battle_RunThenWaitIfModeZero_73(args...) Func_02007ebe(args)
#define Motion_CallWaitAnim_31(args...) Func_02007e6e(args)
#define Battle_WaitMode0_84(args...) Func_02007dec(args)
#define Scene_GetRecord_4_02002778(args...) Func_02007e22(args)
#define Motion_SetPosReset_7(a0, a1, a2) Call3(Func_02007e6a, a0, a1, a2)
#define Battle_WaitMode0_85(args...) Func_02007e10(args)
#define Scene_GetRecord_5_02002778(args...) Func_02007e46_a(args)
#define Battle_WaitMode0_86(args...) Func_02007e26(args)
#define Battle_WaitMode0_87(args...) Func_02007e30(args)
#define Motion_CallWaitAnim_32(args...) Func_02007ec8(args)
#define Battle_WaitMode0_88(args...) Func_02007e46_b(args)
#define Battle_RunThenWaitIfModeZero_74(args...) Func_02007f30(args)
#define Object_SetModeById_15_02002778(args...) Func_02007ed8(args)
#define Object_SetModeById_16_02002778(args...) Func_02007ee0(args)
#define Object_SetModeById_17_02002778(args...) Func_02007ee8(args)
#define Motion_CallWaitAnim_33(args...) Func_02007ef8(args)
#define Battle_WaitMode0_89(args...) Func_02007e76(args)
#define Motion_ArmCb_73(a0, a1, a2) Call3(Func_02007f72, a0, a1, a2)
#define Motion_ArmCb_74(a0, a1, a2) Call3(Func_02007f7e, a0, a1, a2)
#define Motion_ArmCb_75(a0, a1, a2) Call3(Func_02007f8a, a0, a1, a2)
#define Motion_ArmCb_76(a0, a1, a2) Call3(Func_02007f96, a0, a1, a2)
#define Motion_ArmCb_77(a0, a1, a2) Call3(Func_02007fa2, a0, a1, a2)
#define Battle_WaitMode0_90(args...) Func_02007eb8(args)
#define Motion_SetPosReset_8(a0, a1, a2) Call3(Func_02007f26, a0, a1, a2)
#define Motion_SetVarCbObj_16(args...) Func_02007f6e(args)
#define Battle_WaitMode0_91(args...) Func_02007ed4(args)
#define Motion_ArmCb_78(a0, a1, a2) Call3(Func_02007fd0, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_75(args...) Func_02007fca(args)
#define Motion_CallWaitAnim_34(args...) Func_02007f7a(args)
#define Battle_WaitMode0_92(args...) Func_02007ef8_a(args)
#define Battle_RunThenWaitIfModeZero_76(args...) Func_02007fe2(args)
#define Motion_SetSpeed_10(a0, a1, a2) Call3(Func_02007f50, a0, a1, a2)
#define Motion_SetPosReset_9(a0, a1, a2) Call3(Func_02007f7e_a, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_77(args...) Func_02008008(args)
#define Motion_SetSpeed_11(a0, a1, a2) Call3(Func_02007f72_a, a0, a1, a2)
#define Motion_SetSpeed_12(a0, a1, a2) Call3(Func_02007f7c, a0, a1, a2)
#define Motion_ResetPosMode2_7(a0, a1, a2) Call3(Func_02007fa2_a, a0, a1, a2)
#define Motion_ResetPosMode2_8(a0, a1, a2) Call3(Func_02007fb0, a0, a1, a2)
#define Motion_SetPosReset_10(a0, a1, a2) Call3(Func_02007fc6, a0, a1, a2)
#define Motion_CommitPos_8(args...) Func_02007fdc(args)
#define Motion_ResetPosMode2_9(a0, a1, a2) Call3(Func_02007fd2, a0, a1, a2)
#define Motion_SetHPosTerrain_5(args...) Func_02007ffc(args)
#define Motion_CommitPos_9(args...) Func_02007ffa(args)
#define Motion_SetHPosTerrain_6(args...) Func_0200800c(args)
#define Motion_CommitPos_10(args...) Func_0200800a(args)
#define Motion_SetHPosTerrain_7(args...) Func_0200801c(args)
#define Motion_SetPosReset_11(a0, a1, a2) Call3(Func_02008012, a0, a1, a2)
#define BattleFx_SpawnLinked_51(a0, a1, a2) Call3(Func_020080b4, a0, a1, a2)
#define Motion_SetVarCbObj_17(args...) Func_02008064(args)
#define Battle_WaitMode0_93(args...) Func_02007fca_a(args)
#define Motion_ArmCb_79(a0, a1, a2) Call3(Func_020080c6, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_78(args...) Func_020080c0(args)
#define Motion_ArmCb_80(args...) Func_020080da(args)
#define Motion_ArmCb_81(a0, a1, a2) Call3(Func_020080e6, a0, a1, a2)
#define Motion_CallWaitAnim_35(args...) Func_02008086(args)
#define Battle_WaitMode0_94(args...) Func_02008004(args)
#define Battle_RunThenWaitIfModeZero_79(args...) Func_020080ee(args)
#define BattleFx_SpawnLinked_52(a0, a1, a2) Call3(Func_02008110, a0, a1, a2)
#define Motion_SetVarCbObj_18(args...) Func_020080c0_a(args)
#define Motion_ArmCb_82(a0, a1, a2) Call3(Func_0200811c, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_5(a0, a1) Value2(Func_0200811c_a, a0, a1)
#define Motion_CallWaitAnim_36(args...) Func_020080c4(args)
#define Battle_WaitMode0_95(args...) Func_02008042(args)
#define Motion_SetVarCbObj_19(args...) Func_020080ea(args)
#define Battle_WaitMode0_96(args...) Func_02008050(args)
#define Motion_ArmCb_83(a0, a1, a2) Call3(Func_0200814c, a0, a1, a2)
#define Motion_ArmCb_84(a0, a1, a2) Call3(Func_02008158, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_80(args...) Func_02008152(args)
#define BattleFx_SpawnLinked_53(a0, a1, a2) Call3(Func_02008174, a0, a1, a2)
#define BattleFx_SpawnLinked_54(a0, a1, a2) Call3(Func_0200817e, a0, a1, a2)
#define Motion_CallWaitAnim_37(args...) Func_02008116(args)
#define Battle_WaitMode0_97(args...) Func_02008094(args)
#define Battle_RunThenWaitIfModeZero_81(args...) Func_0200817e_a(args)
#define BattleFx_SpawnLinked_55(a0, a1, a2) Call3(Func_020081a2, a0, a1, a2)
#define BattleFx_SpawnLinked_56(a0, a1, a2) Call3(Func_020081ae, a0, a1, a2)
#define Motion_SetSpeed_13(a0, a1) Value2(Func_0200818e, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(args...) Func_020080e6_a(args)
#define Battle_WaitMode0_98(args...) Func_020080d0(args)
#define Motion_CallWaitAnim_38(args...) Func_02008160(args)
#define Battle_WaitMode0_99(args...) Func_020080de(args)
#define Battle_RunThenWaitIfModeZero_82(args...) Func_020081c8(args)
#define Battle_WaitMode0_100(args...) Func_02008100(args)
#define Motion_CallWaitAnim_39(args...) Func_020081a0(args)
#define Battle_WaitMode0_101(args...) Func_0200811e(args)
#define Battle_RunThenWaitIfModeZero_83(args...) Func_02008208(args)
#define Motion_SetVarCbObj_20(args...) Func_020081d0(args)
#define Battle_RunThenWaitIfModeZero_84(args...) Func_0200821a(args)
#define Object_SetModeById_18_02002778(args...) Func_020081c2(args)
#define Motion_CallWaitAnim_40(args...) Func_020081d2(args)
#define Battle_WaitMode0_102(args...) Func_02008150(args)
#define Motion_ResetPosMode2_10(a0, a1, a2) Call3(Func_020081b6, a0, a1, a2)
#define Motion_SetPosReset_12(a0, a1, a2) Call3(Func_020081cc, a0, a1, a2)
#define Motion_SetHPosTerrain_8(args...) Func_020081ee(args)
#define Motion_CommitPos_11(args...) Func_020081ec(args)
#define Motion_SetHPosTerrain_9(args...) Func_020081fe(args)
#define Battle_WaitMode0_103(args...) Func_0200818c(args)
#define ObjectMotion_SetAngleToward_12_02002778(args...) Func_0200823e(args)
#define Battle_WaitMode0_104(args...) Func_0200819c(args)
#define ObjectMotion_SetAngleToward_13_02002778(args...) Func_0200824e(args)
#define Battle_RunThenWaitIfModeZero_85(args...) Func_02008290(args)
#define BattleFx_SpawnLinked_57(a0, a1, a2) Call3(Func_020082b4, a0, a1, a2)
#define Motion_SetVarCb_23(args...) Func_0200825c(args)
#define Battle_WaitMode0_105(a0, a1) Call2(Func_020082ce, a0, a1)
#define Battle_WaitMode0_106(args...) Func_020081d4(args)
#define Motion_CallWaitAnim_41(args...) Func_02008264(args)
#define Battle_WaitMode0_107(args...) Func_020081e2(args)
#define Battle_RunThenWaitIfModeZero_86(args...) Func_020082cc(args)
#define Motion_SetPosReset_13(a0, a1, a2) Call3(Func_0200825a, a0, a1, a2)
#define Motion_SetHPosTerrain_10(args...) Func_0200827c(args)
#define Battle_SchedShoulder_1(args...) Func_02008218(args)
#define GameFlag_Set_1_02002778(a0) Call1(Func_020081fe_a, a0)
#define FieldScene_RunScene39e_02002778 Func_02002778
#define FieldScene_RunScene39e_020027ec Func_020027ec
#define Scene_RunRoofEnsembleSequence Func_02002ad0

struct EffectRec {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
};

struct EffectWork {
    u8 pad[0x50];
    struct EffectRec *rec;
};

/*
 * resource_39e spawn/copy owner at 0x0200013c.
 *
 * Its complete 472-byte extent is the high-register body through the
 * three-word pool immediately before the independent owner at 0x02000314.
 * This is the locally witnessed configurable spawn/copy family: its only
 * 39e-specific identities are the table at 0x0200c62c, the ten in-image
 * call veneers below, and the installed callback 0x02008105.
 */
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Options {
    u8 mode_bits;
    u8 pad01[3];
    s32 mode;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct Descriptor {
    s32 pad00[3];
    s32 duration;
};

struct SceneRecordHeading {
    u8 pad[6];
    u16 heading;
};

struct SceneActor_02001334 {
    u8 unknown_00[6];
    u16 facing;
};

struct Params {
    s32 unused0;
    s32 field1;
    s32 field2;
    s32 field3;
    u8 pad[24];
};

struct Descriptor_02000484 {
    u32 field0;
    u8 unused[12];
    u32 field16;
    u32 field20;
    u16 field24;
    u16 unused26;
    u32 field28;
    u8 unused32[8];
};

struct Descriptor_020041ec {
    u32 field0;
    u32 field4;
    u8 unused8[16];
    u16 field24;
    u8 unused26[6];
    u8 unused32[8];
};

extern struct Descriptor *Data_0200c62c[];
extern s16 Data_02000240[];
extern u8 Value_0000003c;
extern u8 Data_0200c7a8[];
extern u8 Data_0200c838[];
extern u8 Data_0200c8c8[];
extern u8 Data_0200cb90[];
extern u8 Data_0200d184[];
extern u8 Data_0200cd40[];
extern s32 Data_03001e40;
extern u8 Data_000017e0[];

void *Func_020043e6(s32, s32, s32, s32);
void Func_02004424(void *, s32);
void Func_0200443c(void *, s32);
void Func_0200451c(void *, s32);
void *Func_0200443e(s32, s32, s32, s32);
void Func_02004480(void *, s32);
void Func_02004578(void *, s32);
struct Effect *Func_02004562();
struct Effect *Func_02004510();
void Func_0200451a();
void Func_02004534();
void Func_020046ae();
s32 Func_020045b8();
s32 Func_020045d0();
s32 Func_020045de();
void Func_02004634();
void Func_02004644();
void Func_020046be(void *, s32);
void *Func_02004730(s32);
s16 Func_020046a2(s32, s32);
s32 Func_0200684a();
s32 Func_02006854();
void Func_02006864();
void Func_02006858();
void Func_0200688c();
void Func_02006a2c();
void Func_0200684e();
void Func_020068dc(s32, s32, s32);
void Func_02006908(s32, s32, s32);
void Func_0200690c();
void Func_020068ba();
void Func_020069f0();
void Func_020068d4();
s32 Func_020047e2();
void Func_020047f4();
void Func_0200481a();
s32 Func_0200483e();
void Func_02004858();
void Func_020048ba();
s32 Func_020048e6();
void Func_0200491c();
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
void Func_020062de_a();
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
void Func_02006340_a();
void Func_02006350();
void Func_02006350_a();
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
void Func_02006476_a();
void Func_02006476_b();
void Func_0200647e();
void Func_02006480();
void Func_02006486();
void Func_02006486_a();
void Func_0200648e();
void Func_02006492();
void Func_02006496();
void Func_02006496_a();
void Func_02006498();
void Func_0200649e();
void Func_020064a6();
void Func_020064a6_a();
void Func_020064ae();
void Func_020064ae_a();
void Func_020064b6();
void Func_020064b6_a();
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
void Func_02006542_a();
s32 Func_0200654e();
s32 Func_0200654e_a();
void Func_02006568();
void Func_02006570();
void Func_02006578();
void Func_02006588();
void Func_02006588_a();
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
void Func_020065d8_a();
void Func_020065f0();
void Func_02006604();
void Func_02006606();
void Func_02006610();
void Func_02006612();
void Func_0200661a();
void Func_02006622();
void Func_02006628();
void Func_0200662c();
void Func_0200662c_a();
void Func_02006636();
void Func_02006640();
void Func_0200664a();
void Func_0200664a_a();
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
void Func_020066f4_a();
void Func_020066fc();
void Func_02006704();
void Func_0200670c();
void Func_02006714();
s32 Func_02006724();
s32 Func_02006724_a();
s32 Func_02006724_b();
void Func_02006734();
void Func_02006744();
void Func_0200674a();
void Func_0200674c();
void Func_0200675c();
void Func_02006786();
void Func_0200678e();
void Func_0200678e_a();
void Func_02006796();
void Func_0200679a();
void Func_0200679e();
void Func_020067a4();
void Func_020067a6();
void Func_020067a6_a();
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
void Func_020067fa_a();
void Func_020067fc();
s32 Func_020067fe();
void Func_02006806();
s32 Func_0200680a();
void Func_0200680c();
void Func_0200680e();
s32 Func_02006816();
void Func_02006840();
void Func_0200685a();
void Func_0200685a_a();
void Func_02006864_a();
void Func_020068a2();
void Func_02006900();
void Func_0200699c();
void Func_02006a70();
void Func_02006ab4();
void Func_02006ab6();
s32 Func_02006ae8();
void Func_02006b08();
void Func_02006b12();
void Func_02006b1a();
void Func_02006b20();
void Func_02006a34();
void Func_02006a92();
void Func_02006ab8();
void Func_02006adc();
void Func_02006b06();
void Func_02006b30();
s32 Func_02006b36();
void Func_02006b5a();
void Func_02006b66();
void Func_02006b68();
void Func_02006bb8();
void Func_02006c30();
void Func_02006c4c();
void Func_02006c76();
void Func_02006c86();
void Func_0200a5b9();
void Func_02004864(void);
s32 Func_0200484a(s32);
void Func_02004934(s32);
s32 Func_0200495c(s32, s32);
void Func_02004888(void);
void Func_02004948(s32);
s32 Func_02004958(s32, s32);
s32 Func_020048b0(s32, s32);
s32 Func_0200497c(s32, s32);
s32 Func_020048d4(s32, s32);
void Func_020049a6(s32, s32);
void Func_020048e2(void);
void Func_02004e9c(void);
void Func_02004f62(s32);
s32 Func_02004f8a(s32, s32);
void Func_02004eb6(void);
void Func_02004ebc(void);
void Func_02004f82(s32);
s32 Func_02004faa(s32, s32);
void Func_02004ed6(void);
void Func_02004e58(s32 arg0);
s32 Func_02004f16(s32 flagId);
s32 Func_02004f28(s32 flagId);
void Func_02004edc(void);
void Func_02004fa0(void);
u8 *Func_02004f0a(s32 slot);
u8 *Func_02004f1e(s32 slot);
u8 *Func_02004f2a(s32 slot);
u8 *Func_02004f34(s32 slot);
u8 *Func_02004f40(s32 slot);
u8 *Func_02004f48(s32 slot);
u8 *Func_02004fa2(s32 slot);
u8 *Func_02004fae(s32 slot);
void Func_02004fd4(s32 slot, u8 *descriptor);
void Func_02004fd0(s32 slot, s32 a1, s32 a2);
void Func_02005000(s32 dialogueId);
void Func_02005012(s32 dialogueId);
void Func_0200502c(s32 dialogueId);
void Func_02005044(s32 slot, s32 a1);
void Func_02004fb4(void);
void Func_02004fce(void);
void Func_0200507a(s32 dialogueId);
s32 Func_020050a2(s32 slot, s32 arg1);
u8 *Func_02005004();
void Func_02004f56();
s32 Func_02004f8c();
s32 Func_02004f96();
void Func_020051cc();
void Func_02000dae();
void Func_02000dc6();
void Func_02000de4();
u8 *Func_020051c6(s32 id);
void Func_02005118(s32 arg0);
void Func_02005370(s32 sound_id);
void Func_02000f4c(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02000f62(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02000f86(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02000fa2(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
s32 Func_020056d0(s32 flagId);
void Func_020056c0(void);
void Func_0200570c(void);
u8 *Func_020056ee(s32 slot);
u8 *Func_020056fe(s32 slot);
void Func_02001efc(void);
void Func_02001fe2(void);
void Func_0200247c(void);
void Func_02002350(s32 arg0);
void Func_02005722(s32 frames);
void Func_0200571c(void);
void Func_02005784(void);
struct SceneActor_02001334 *Func_0200573e(s32 slot);
void Func_0200579c(s32 slot, s32 mode);
void Func_020057ea(s32 slot, s32 mode);
void Func_02001f68(void);
void Func_0200213c(void);
void Func_02002062(void);
void Func_020021f6(void);
void Func_020023c8(s32 variant);
u8 *Func_020057c2();
void Func_020057a0();
void Func_0200226c(void);
void Func_02001fde(void);
void Func_020021b2(void);
void Func_020020d8(void);
void Func_020058f4();
void Func_02005914();
void Func_02005910(void);
s32 Func_020057e6();
s32 Func_020057f0();
void Func_0200246a();
void Func_020025a4(void);
void Func_0200582c();
s32 Func_0200580c();
void Func_02002486();
s32 Func_0200581e();
void Func_020028fe(void);
void Func_0200322c(void);
void Func_02005858();
void Func_02006130(void);
void Func_02006148();
void Func_02006148_a();
s32 Func_0200615e(s32 arg0, s32 arg1);
void Func_0200616c(void);
void Func_0200618c(void);
void Func_020061f6(s32 dialogueId);
s32 Func_02006206(s32 slot, s32 arg1);
void Func_02006232(s32 slot, s32 arg1, s32 arg2);
void Func_02006258(s32 slot, s32 arg1, s32 arg2);
void Func_02006844(void);
void Func_0200685e(void);
void Func_0200690a(s32 dialogue_id);
s32 Func_02006932(s32 id, s32 arg1);
void Func_020068f0();
void Func_0200691c();
u8 *Func_02006912();
void Func_020069c8();
void Func_020069e0();
void Func_02006a98();
void Func_0200692c(void);
void Func_0200694e(void);
void Func_020069cc(s32 id, s32 arg1);
void Func_020069fa(s32 dialogue_id);
void Func_02006a12(s32 id, s32 arg1);
u8 *Func_020069c4(s32 arg0);
void Func_02006ac8(s32 id, s32 arg1);
void Func_02006ad2(s32 id, s32 arg1);
s32 Func_02006928(void);
s32 Func_02006944(void);
void Func_02004cf2(s32 arg0, s32 arg1);
void Func_02004d00(s32 arg0, s32 arg1);
void Func_02004d12(s32 arg0, s32 arg1);
void Func_02006ce6(s32 sound_id);
void Func_02006c74(s32 arg0);
void Func_020084b0();
void Func_02008638();
void Func_0200864c();
void Func_02008666();
void Func_0200845a();
void Func_02008584(s32 id, s32 arg1);
void Func_02008672(s32 arg0);
void Func_0200868e(void);
u8 *Func_0200854c(s32 id);
void Func_02008540(s32 frames);
void Func_020086f6(s32 sound_id);
void Func_020042d6(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02008578(s32 frames);
void Func_0200866c(s32 arg0, s32 arg1);
void Func_02008674(s32 arg0, s32 arg1);
u8 *Func_02008606(s32 id);
u8 *Func_0200863a(s32 id);
u8 *Func_02008648(s32 id);
u8 *Func_020086fa(s32 id);
u8 *Func_02008704(s32 id);
void Func_020085e0(s32 frames);
void Func_020085fe(s32 frames);
void Func_02008612(s32 frames);
void Func_020086ae(s32 frames);
void Func_020086c4(s32 frames);
void Func_02008798(s32 id, s32 arg1);
void Func_020087a0(s32 id, s32 arg1);
void Func_0200872a(s32 arg0, s32 arg1);
void Func_020087f0(s32 arg0, s32 arg1);
void Func_02008740(s32 arg0);
void Func_02008806(s32 arg0);
void Func_0200860e(s32 arg0);
s32 Func_020085c6(void);
s32 Func_020085de(void);
s32 Func_020085f2(void);
void Func_02008778(s32 sound_id);
void Func_020087ac(s32 sound_id);
void Func_020087e2(s32 sound_id);
void Func_02008850(s32 sound_id);
void Func_020043fe(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02005934();
void Func_0200585e();
void Func_02005874();
s32 Func_02006026();
u8 *Func_020059c6();
void Func_02005e26();
void Func_02005976();
void Func_0200615c();
void Func_02005b04();
void Func_020059a0();
void Func_02005b30();
void Func_02005e60();
void Func_02006184();
void Func_02005c54();
void Func_02005ae8();
void Func_02005a1c();
void Func_02005926();
void Func_02005990();
s32 Func_020060ce();
void Func_020059b2();
void Func_02005a06();
void Func_02005c94();
void Func_0200599c();
void Func_02005a34();
void Func_02005c80();
void Func_02006b20_a();
void Func_02006b58();
s32 Func_02006b96();
void Func_02006b9e();
void Func_02006bb6();
void Func_02006bb8_a();
void Func_02006c00();
void Func_02006c0c();
void Func_02006cd4();
void Func_02006cfe();
void Func_02006bcc();
void Func_02006c00_a();
void Func_02006c12();
void Func_02006c18();
void Func_02006c36();
s32 Func_02006c4e();
s32 Func_02006c60();
void Func_02006c66();
void Func_02006c6a();
void Func_02006c74_a();
void Func_02006ca0();
void Func_02006ca2();
void Func_02006cb0();
s32 Func_02006cb8();
void Func_02006cbe();
void Func_02006cd0();
void Func_02006cde();
void Func_02006ce8();
void Func_02006cf6();
void Func_02006d04();
void Func_02006d0c();
void Func_02006d12();
void Func_02006d26();
void Func_02006d2e();
void Func_02006d32();
void Func_02006d38();
void Func_02006d40();
void Func_02006d54();
void Func_02006d5e();
s32 Func_02006d60();
s32 Func_02006d6a();
void Func_02006d78();
void Func_02006d80();
void Func_02006d86();
s32 Func_02006d96();
void Func_02006d9a();
void Func_02006da8();
void Func_02006db6();
void Func_02006dc0();
void Func_02006dd0();
void Func_02006dd2();
void Func_02006dd6();
void Func_02006dde();
s32 Func_02006de6();
void Func_02006dee();
void Func_02006df8();
void Func_02006e00();
void Func_02006e10();
s32 Func_02006e12();
void Func_02006e2e();
void Func_02006e30();
void Func_02006e32();
void Func_02006e38();
void Func_02006e3e();
void Func_02006e48();
void Func_02006e58();
void Func_02006e5a();
void Func_02006e60();
void Func_02006e6a();
void Func_02006e6a_a();
void Func_02006e6c();
void Func_02006e6e();
void Func_02006e84();
s32 Func_02006e8e();
void Func_02006e90();
void Func_02006ea8();
void Func_02006eb2();
void Func_02006ec8();
void Func_02006ee2();
void Func_02006ef0();
void Func_02006ef6();
void Func_02006f02();
void Func_02006f1a();
void Func_02006f32();
void Func_02006eb0();
void Func_02006ee6();
void Func_02006ef2();
void Func_02006f10();
void Func_02006f18();
void Func_02006f2a();
void Func_02006f3e();
void Func_02006f4e();
void Func_02006f80();
u8 *Func_02006f90();
void Func_02006f92();
u8 *Func_02006fa4();
void Func_02006fac();
void Func_02006fb4();
u8 *Func_02006fb8();
void Func_02006fc8();
void Func_02006fd6();
void Func_02006fe4();
void Func_02006fe6();
void Func_02006fe8();
void Func_02006ff2();
void Func_02006ff2_a();
void Func_02006ffa();
void Func_02007000();
void Func_0200700c();
void Func_0200700e();
void Func_0200700e_a();
void Func_02007014();
void Func_02007014_a();
void Func_02007028();
void Func_02007030();
void Func_02007034();
void Func_0200703c();
void Func_02007042();
void Func_0200704a();
void Func_02007050();
void Func_0200705e();
void Func_0200707c();
void Func_0200707e();
void Func_02007082();
void Func_02007088();
void Func_0200708c();
void Func_0200709a();
void Func_020070c6();
void Func_0200710e();
void Func_02007114();
void Func_02007126();
void Func_0200713e();
void Func_0200713e_a();
void Func_02007148();
void Func_02007148_a();
void Func_02007152();
void Func_0200715c();
void Func_02007160();
void Func_02007160_a();
void Func_02007168();
void Func_02007176();
void Func_02007176_a();
void Func_02007180();
void Func_02007192();
void Func_02007198();
void Func_02007198_a();
void Func_020071a6();
void Func_020071b0();
void Func_020071c6();
void Func_020071c6_a();
void Func_020071d4();
void Func_020071dc();
void Func_020071e4();
void Func_020071ea();
void Func_020071fe();
void Func_02007202();
void Func_0200720a();
void Func_02007216();
void Func_0200721e();
void Func_0200722c();
void Func_02007232();
void Func_02007238();
void Func_0200723a();
void Func_02007246();
void Func_02007252();
void Func_02007252_a();
void Func_02007254();
void Func_0200725e();
void Func_02007266();
void Func_0200726a();
void Func_02007282();
void Func_02007284();
void Func_0200728c();
void Func_02007294();
void Func_020072ae();
void Func_020072b2();
void Func_020072b8();
void Func_020072be();
void Func_020072c6();
void Func_020072da();
void Func_020072e8();
void Func_020072f0();
void Func_020072f4();
void Func_020072f6();
void Func_02007306();
void Func_0200730c();
void Func_02007316();
void Func_0200731a();
void Func_02007332();
void Func_02007336();
void Func_0200733e();
void Func_02007340();
void Func_02007350();
void Func_02007356();
void Func_0200735c();
void Func_02007364();
void Func_0200736e();
void Func_02007376();
void Func_02007378();
void Func_0200738e();
void Func_02007392();
void Func_02007394();
void Func_020073a2();
void Func_020073ac();
void Func_020073ba();
void Func_020073c6();
void Func_020073d2();
void Func_020073d4();
void Func_020073de();
void Func_020073e2();
void Func_020073ea();
void Func_02007404();
void Func_0200740e();
void Func_02007412();
void Func_02007424();
void Func_02007428();
void Func_0200742a();
void Func_0200743e();
void Func_02007440();
void Func_0200744c();
void Func_0200744c_a();
void Func_0200744e();
void Func_02007454();
void Func_02007464();
void Func_02007466();
void Func_02007472();
void Func_0200747e();
void Func_02007484();
void Func_0200748c();
void Func_02007490();
void Func_02007494();
void Func_020074a4();
void Func_020074b6();
void Func_020074b8();
void Func_020074bc();
void Func_020074c0();
void Func_020074c8();
void Func_020074ce();
void Func_020074ce_a();
void Func_020074d4();
void Func_020074d8();
void Func_020074f0();
void Func_020074f6();
void Func_020074f8();
void Func_020074fe();
void Func_020074fe_a();
void Func_02007506();
void Func_02007510();
void Func_02007516();
void Func_0200751e();
void Func_02007536();
void Func_02007548();
void Func_0200754e();
void Func_02007550();
void Func_02007558();
void Func_02007566();
void Func_0200757e();
void Func_02007594();
void Func_020075a0();
void Func_020075a0_a();
void Func_020075a0_b();
void Func_020075a2();
void Func_020075a2_a();
void Func_020075b8();
void Func_020075d2();
void Func_020075d6();
void Func_020075da();
void Func_020075da_a();
void Func_020075e2();
void Func_020075e4();
void Func_020075e8();
void Func_020075f2();
void Func_020075f4();
void Func_020075fa();
void Func_02007608();
void Func_02007612();
void Func_02007630();
void Func_0200763c();
void Func_0200763e();
void Func_02007650();
void Func_02007656();
void Func_0200765e();
void Func_02007666();
void Func_0200766a();
void Func_02007672();
void Func_02007676();
void Func_02007676_a();
void Func_02007676_b();
void Func_0200768c();
void Func_020076a8();
void Func_020076aa();
void Func_020076ac();
void Func_020076ae();
void Func_020076b2();
void Func_020076b4();
void Func_020076ba();
void Func_020076c0();
void Func_020076c8();
void Func_020076ca();
void Func_020076ea();
void Func_020076ec();
void Func_020076f0();
void Func_02007708();
void Func_02007728();
void Func_02007736();
void Func_0200773c();
void Func_0200774a();
void Func_02007754();
void Func_02007754_a();
void Func_0200775e();
void Func_0200777e();
void Func_02007788();
void Func_02007792();
void Func_02007796();
void Func_0200779c();
void Func_020077a2();
void Func_020077a6();
void Func_020077b2();
void Func_020077b2_a();
void Func_020077bc();
void Func_020077be();
void Func_020077c4();
void Func_020077c6();
void Func_020077ce();
void Func_020077d4();
void Func_020077d6();
void Func_020077d6_a();
void Func_020077d8();
void Func_020077de();
void Func_020077e8();
void Func_020077f2();
void Func_020077f2_a();
void Func_02007850();
void Func_0200785c();
void Func_02007868();
s32 Func_0200786e();
void Func_0200788a();
void Func_02007896();
void Func_020078a2();
void Func_020078ae();
void Func_020078b8();
void Func_020078b8_a();
void Func_020078cc();
void Func_020078d2();
void Func_020078e4();
void Func_020078e8();
void Func_020078f2();
void Func_020078f4();
void Func_020078fe();
void Func_0200790c();
void Func_02007910();
void Func_0200791a();
void Func_02007928();
void Func_0200792c();
void Func_02007930();
void Func_0200793a();
void Func_02007946();
void Func_0200794e();
void Func_0200794e_a();
void Func_02007956();
void Func_02007958();
void Func_0200795e();
void Func_02007962();
void Func_02007966();
void Func_02007966_a();
void Func_0200796c();
void Func_02007980();
void Func_02007980_a();
void Func_0200798a();
void Func_0200798c();
void Func_02007994();
void Func_0200799e();
void Func_020079ac();
void Func_020079dc();
void Func_020079f0();
void Func_020079f6();
void Func_02007a02();
void Func_02007a02_a();
void Func_02007a0e();
void Func_02007a10();
void Func_02007a1a();
void Func_02007a2c();
void Func_02007a30();
void Func_02007a38();
void Func_02007a44();
void Func_02007a50();
void Func_02007a62();
void Func_02007a6a();
void Func_02007a7a();
void Func_02007a7c();
void Func_02007a82();
void Func_02007aa6();
void Func_02007aa6_a();
void Func_02007aa8();
void Func_02007ab2();
void Func_02007abe();
void Func_02007ac0();
void Func_02007aca();
void Func_02007ad6();
void Func_02007ad6_a();
void Func_02007ae2();
void Func_02007aec();
void Func_02007af6();
void Func_02007af8();
void Func_02007b00();
void Func_02007b04();
void Func_02007b10();
void Func_02007b1a();
void Func_02007b3c();
void Func_02007b4a();
void Func_02007b58();
void Func_02007b5a();
void Func_02007b78();
void Func_02007b7e();
void Func_02007b7e_a();
void Func_02007b82();
void Func_02007b8c();
void Func_02007ba2();
void Func_02007ba4();
void Func_02007baa();
void Func_02007bbe();
void Func_02007bca();
void Func_02007bcc();
void Func_02007bd0();
void Func_02007be0();
void Func_02007bf0();
void Func_02007bf4();
void Func_02007c02();
void Func_02007c0a();
void Func_02007c0e();
void Func_02007c12();
void Func_02007c18();
void Func_02007c20();
void Func_02007c26();
void Func_02007c28();
void Func_02007c2e();
void Func_02007c36();
void Func_02007c42();
void Func_02007c44();
void Func_02007c46();
void Func_02007c50();
void Func_02007c5c();
void Func_02007c62();
void Func_02007c68();
void Func_02007c6e();
void Func_02007c76();
void Func_02007c7c();
void Func_02007c8c();
void Func_02007c8e();
void Func_02007c9a();
void Func_02007c9a_a();
void Func_02007ca2();
void Func_02007ca8();
void Func_02007cb0();
void Func_02007cba();
void Func_02007cbe();
void Func_02007cc8();
void Func_02007cce();
void Func_02007cda();
void Func_02007cdc();
void Func_02007ce4();
void Func_02007ce8();
void Func_02007cea();
void Func_02007cf2();
void Func_02007cf8();
void Func_02007d0e();
void Func_02007d30();
void Func_02007d38();
void Func_02007d54();
void Func_02007d66();
void Func_02007d68();
void Func_02007d7a();
void Func_02007d7e();
void Func_02007d96();
void Func_02007d9c();
void Func_02007db6();
void Func_02007dc2();
void Func_02007dc6();
void Func_02007dce();
void Func_02007dd4();
void Func_02007dd6();
void Func_02007de2();
void Func_02007dec();
void Func_02007dfa();
void Func_02007e0c();
void Func_02007e10();
void Func_02007e14();
void Func_02007e16();
void Func_02007e1c();
u8 *Func_02007e22();
void Func_02007e26();
void Func_02007e2c();
void Func_02007e30();
void Func_02007e42();
void Func_02007e4c();
void Func_02007e52();
void Func_02007e56();
void Func_02007e60();
void Func_02007e6a();
void Func_02007e6e();
void Func_02007e76();
void Func_02007e94();
void Func_02007e9e();
void Func_02007eaa();
void Func_02007eb6();
void Func_02007eb8();
void Func_02007ebe();
void Func_02007ec8();
void Func_02007ed4();
void Func_02007ed8();
void Func_02007ee0();
void Func_02007ee8();
void Func_02007ef8();
void Func_02007ef8_a();
void Func_02007f26();
void Func_02007f30();
void Func_02007f50();
void Func_02007f6e();
void Func_02007f72();
void Func_02007f72_a();
void Func_02007f7a();
void Func_02007f7c();
void Func_02007f7e();
void Func_02007f7e_a();
void Func_02007f8a();
void Func_02007f96();
void Func_02007fa2();
void Func_02007fa2_a();
void Func_02007fb0();
void Func_02007fc6();
void Func_02007fca();
void Func_02007fca_a();
void Func_02007fd0();
void Func_02007fd2();
void Func_02007fdc();
void Func_02007fe2();
void Func_02007ffa();
void Func_02007ffc();
void Func_02008004();
void Func_02008008();
void Func_0200800a();
void Func_0200800c();
void Func_02008012();
void Func_0200801c();
void Func_02008042();
void Func_02008050();
void Func_02008064();
void Func_02008086();
void Func_02008094();
void Func_020080b4();
void Func_020080c0();
void Func_020080c0_a();
void Func_020080c4();
void Func_020080c6();
void Func_020080d0();
void Func_020080da();
void Func_020080de();
s32 Func_020080e6();
s32 Func_020080e6_a();
void Func_020080ea();
void Func_020080ee();
void Func_02008100();
void Func_02008110();
void Func_02008116();
s32 Func_0200811c();
s32 Func_0200811c_a();
void Func_0200811e();
void Func_0200814c();
void Func_02008150();
void Func_02008152();
void Func_02008158();
void Func_02008160();
void Func_02008174();
void Func_0200817e();
void Func_0200817e_a();
void Func_0200818c();
s32 Func_0200818e();
void Func_0200819c();
void Func_020081a0();
void Func_020081a2();
void Func_020081ae();
void Func_020081b6();
void Func_020081c2();
void Func_020081c8();
void Func_020081cc();
void Func_020081d0();
void Func_020081d2();
void Func_020081d4();
void Func_020081e2();
void Func_020081ec();
void Func_020081ee();
void Func_020081fe();
void Func_020081fe_a();
void Func_02008208();
void Func_02008218();
void Func_0200821a();
void Func_0200823e();
void Func_0200824e();
void Func_0200825a();
void Func_0200825c();
void Func_02008264();
void Func_0200827c();
void Func_02008290();
void Func_020082b4();
void Func_020082cc();
void Func_020082ce();
u8 *Func_02007e46();
u8 *Func_02007e46_a();
u8 *Func_02007e46_b();

/*
 * resource_39e owner at 0x02001494, 2236 bytes.
 *
 * Second ensemble beat of this overlay's scene script: 233 calls into the
 * shared scene-script helper block, then a two-stage story branch.
 *
 *   - the opening call sets story flag 0x89a;
 *   - stage 1 (Func_0808a070(0, 0) == 0) joins with id 16 and sets flag 0x898
 *     -- the flag the dispatcher at 0x020012e0 tests and 0x02001dbc clears;
 *   - stage 1 otherwise bumps the step counter at +472 of the scene work
 *     record and runs stage 2: its ==0 arm joins with id 18 and also sets
 *     0x898, its else arm bumps that counter again and sets flag 0x899.
 *
 * Both join arms share the tail at 0x02001cb4: Func_0808a188(id, 0, 20) with
 * the id carried in r0, then Func_080770c8(0x898).
 *
 * Midway the beat clears then sets bit 0 of byte +90 of record 16 around a
 * Func_0808a0d0 reposition, plays sounds 158/159 with Func_08009178 text
 * calls, runs the slot-19/20 sequence (Func_0808a0f0(19/20, 232 << 16,
 * 168 << 16), record +12 = 0xc0000, +60 = 0x80000000, +24 = 0xcccc, +30 of
 * the +80 sub-object = 0x8000, sound 124) that also closes the owners at
 * 0x02001160 and 0x02001dbc, and ends with Func_0808a098(12, 0x0200c638) as
 * 0x02001dbc does.
 *
 * Complete owner: `push {r5, lr}` at 0x02001494 through `pop {r5} / pop {r0} /
 * bx r0` at 0x02001d42-0x02001d46, two inline pools plus two trailing pool
 * words ending 0x02001d4f; the next owner (0x02001d50, `push {lr}`) follows.
 * Called once from within this overlay.
 *
 * Call binding. This overlay reaches the shared helpers through its own veneer
 * bank, so every emitted call word names a veneer, not the main-image address.
 * The legacy alias a reference site produces is derived from that site's own
 * position, so one alias can name two different veneers and several aliases
 * can name one veneer. Each helper is therefore declared once, under an alias
 * whose reference sites all resolve to that helper's veneer, and is spelled in
 * the body under the main-image function it reaches. The bound veneer -- and
 * so the emitted call word -- depends only on the alias, never on where the
 * call lands in the candidate.
 *
 * Two call forms in the branch tail carry meaning beyond their arguments. The
 * three sites spelled through Value2 keep the callee's result register live
 * across the argument setup, which orders the r0 write after r1; the sites
 * spelled through Call2 discard it and order r0 first. Both forms appear in
 * the sibling beat at 0x02001dbc.
 *
 * The set of bit 0 in byte +90 reads the byte into its own local before the
 * or, as the staged-actor scene at resource_373:02003fb0 does at the same
 * offset; the paired clear needs no local because an and against 0xfe already
 * holds its mask in a word register.
 *
 * Uncertainty: helper roles and argument roles are read from call shape only;
 * ids, dialogue ids and constants are transcribed. 0x0200c77a, 0x0200c790 and
 * 0x0200c638 are overlay-image data pointers whose contents are unidentified.
 */

/* Veneer aliases this overlay binds; see Call binding above. */

/* The main-image helper each veneer above reaches. */

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    Func_020068dc(actor, horizontal, vertical);
}

static __inline__ void SetPosition(s32 actor, s32 horizontal, s32 vertical)
{
    Func_02006908(actor, horizontal, vertical);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Field cleared to 0 on each actor record below. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Newly named raw call sites: each engine function below was reached only
 * through an unresolved veneer island until its own source was read; the
 * per-site macro keeps the site's original calling form. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001dbc(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_0200268c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call2_020026d8(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call precomputes a costly constant into a
 * pseudo that the compiler then shares with later uses in the block. A
 * value-returning site also writes r0 last of its arguments. The same wrappers
 * carry the sibling beat at 0x02001dbc. */
static __inline__ void Call1_02001494(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ u8 *Record1(u8 *(*f)(), s32 a0)
{
    return f(a0);
}

/* The scene step counter at +472 of the shared scene work record. */
static __inline__ void Scene_BumpStep(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + amount);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02002778(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02002778(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call3_020027ec(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call3_02002ad0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

void Func_02002766(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                          u32 flags, u8 *extra);

void SetEffectMode(struct EffectWork *work, u32 mode)
{
    work->rec->f1 = mode;
}

void *NewEffectObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *overlay_object;
    void *object_record;
    s32 flags_mask;

    overlay_object = AcquireOverlayObject(fourth, first, second, third);
    if (overlay_object != NULL) {
        object_record = FIELD_AT_OFFSET(overlay_object, void *, 0x50);
        flags_mask = -0xD;
        FIELD_AT_OFFSET(object_record, u8, 9) = (u8)(flags_mask & FIELD_AT_OFFSET(object_record, u8, 9));
        FIELD_AT_OFFSET(overlay_object, u8, 0x55) = 0;
        FIELD_AT_OFFSET(overlay_object, u8, 0x59) = 8;
        RunOverlayObjectCommand0(overlay_object, 0);
        RunOverlayObjectCommand14(overlay_object, 0xE);
        RunOverlayObjectCommand1(overlay_object, 1);
        return overlay_object;
    }
    return NULL;
}

void *NewFlippedEffectObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = Func_0200443e(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        Func_02004480(result, 0);
        Func_02004578(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
}

void SpawnEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
                   const struct Options *options)
{
    u32 table_offset;
    struct Effect *party;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *mode_block;
    u32 option_bits;
    u16 *tag;
    s32 duration;
    s32 first_delta;
    s32 accumulated;

    party = Func_02004562(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_02004510(options->kind, x, y, z);
    } else {
        effect = Func_02004510(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    mode_block = block;

    Func_0200451a(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Func_02004534(effect, Data_0200c62c[table_offset >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = party->sprite->flags9 & 12;
    block_bits = *(volatile u8 *)&block->flags9;
    flag_mask = ~12;
    block->flags9 = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {
        Func_020046ae(effect, options->mode);
    }

    if ((flags & 0x20000) != 0) {
        effect->flags23 &= 0xfe;
        option_bits = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)mode_block + 9) & flag_mask)
                             | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {
        const struct Descriptor *descriptor =
            Data_0200c62c[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = Func_020045b8(first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = Func_020045d0(first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }
        effect->rate34 = Func_020045de(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Func_02004634(effect, 1);
        Func_02004644(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0) {
        block->angle = options->angle;
    }

    if ((flags & 0x800000) != 0) {
        *tag = options->step;
    }

    if ((flags & 0x1000000) != 0) {
        effect->callback = options->callback;
    }
}

s32 StopXianActor(void *actor)
{
    Func_020046be(actor, 0);
    return 0;
}

s32 FaceXianActorToPlayer(void *actor)
{
    void *player = Func_02004730(0);
    FIELD(actor, u16, 6) = Func_020046a2(FIELD(player, s32, 0x10) - FIELD(actor, s32, 0x10), FIELD(player, s32, 8) - FIELD(actor, s32, 8));
    return 0;
}

s32 GetXianScriptData(void)
{
    if (Data_02000240[224] == (s32)&Value_0000003c) {
        return (s32)Data_0200c7a8;
    }
    return (s32)Data_0200c838;
}

s32 GetXianInitialState(void)
{
    return 0;
}

s32 GetXianMessageData(void)
{
    return (s32)Data_0200c8c8;
}

void FieldScene_RunScene39e_02000414(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_020047f4();
    Call1(Func_020048ba, 0x178a);
    if (Value1(Func_020047e2, 0x890) != 0) {
        bump_step(4);
    }
    Value2(Func_020048e6, 8, 0);
    if (Value2(Func_0200483e, 0, 0) == 0) {
        Call1(Func_0200481a, 0x890);
    } else {
        bump_step(1);
    }
    Func_0200491c(8, 0);
    Func_02004858();
}

void Scene_RunFlag88FBranch(void)
{
    extern u8 *Data_03001ebc;

    Func_02004864();
    if (Func_0200484a(0x88F) != 0) {
        Func_02004934(0x17D6);
        Func_0200495c(12, 0);
        Func_02004888();
    } else {
        Func_02004948(0x1794);
        Func_02004958(12, 0);
        if (Func_020048b0(0, 0) == 1) {
            u16 *q = (u16 *)(Data_03001ebc + 0x1D8);
            q[0] = q[0] + 1;
            Func_0200497c(12, 0);
            if (Func_020048d4(0, 0) == 1) {
                u16 *r = (u16 *)(Data_03001ebc + 0x1D8);
                r[0] = r[0] + 1;
            }
        }
        Func_020049a6(12, 0);
        Func_020048e2();
    }
}

void Scene_ShowDialogue17B1(void)
{
    extern u8 *Data_03001ebc;

    Func_02004e9c();
    Func_02004f62(0x17B1);
    Func_02004f8a(8, 0);
    Func_02004eb6();
}

void Scene_ShowDialogue1825(void)
{
    extern u8 *Data_03001ebc;

    Func_02004ebc();
    Func_02004f82(0x1825);
    Func_02004faa(9, 0);
    Func_02004ed6();
}

void Scene_RunRoofSceneExit(void)
{
    extern u8 *Data_03001ebc;

    OpenSceneExit();

    ClearSceneExitGateAtEntry(12)[91] = 0;

    goto testPendingWork;
waitPendingWork:
        WaitSceneExitStep(1);
testPendingWork:
    if (*(s32 *)(GetSceneExitPendingWork(12) + 12) > 0) {
        goto waitPendingWork;
    }

    *(s32 *)(ResetSceneExitPendingWork(12) + 12) = 0;

    *(s32 *)(SetSceneExitCompletionMode(12) + 60) = 128 << 24;

    *(s32 *)(ClearSceneExitField40(12) + 40) = 0;

    SetSceneExitGate(12)[91] = 1;

    TransitionSceneExitSlot(12, 0, 0);

    if (IsFlag0895Set(0x895) != 0) {
        ShowSceneExitDialogue1a5b(0x1a5b);
    } else if (IsFlag089bSet(0x89b) != 0) {
        ShowSceneExitDialogue189e(0x189e);
    } else {
        ShowSceneExitDialogue182a(0x182a);
    }

    FinalizeSceneExitSlot(12, 0);

    ((struct SceneRecordHeading *)SetSceneExitHeading(12))->heading = 128 << 7;

    ClearSceneExitGateBeforeDescriptor(12)[91] = 0;

    InstallSceneExitDescriptor(12, (u8 *)0x0200c638);
    CloseSceneExit();
}

void Scene_ShowDialogue182D(void)
{
    extern u8 *Data_03001ebc;

    Func_02004fb4();
    Func_0200507a(0x182d);
    Func_020050a2(15, 0);
    Func_02004fce();
}

void Scene_RunForwardArcBurst(void)
{
    extern u8 *Data_03001ebc;

    u8 *record = Func_02005004(19);
    u32 index;
    s32 angle;

    for (index = 8; index > 3; index--) {
        angle = index << 12;
        *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)angle;
        Func_02004f56((index - 4) * 2);
        *(s32 *)(record + 8) += Func_02004f8c(angle)* 6;
        *(s32 *)(record + 16) += Func_02004f96(angle)* 6;
    }

    *(s32 *)(record + 12) = 0x120000;
    *(s32 *)(record + 60) = 0x120000;

    Func_020051cc(227);

    Func_02000dae(*(s32 *)(record + 8) - 0xc0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffffcccd, 0x6666, 0, 0, 0);
    Func_02000dc6(*(s32 *)(record + 8),
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff3334, 0x4ccc, 0, 0, 0);
    Func_02000de4(*(s32 *)(record + 8) + 0xa0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff0000, 0x3333, 0, 0, 0);
}

void Scene_RunDescentBurst(void)
{
    extern u8 *Data_03001ebc;

    u8 *record = Func_020051c6(19);
    u32 i = 0;
    s32 step = 8;
    s32 zero;
    do {
        Func_02005118(step);
        *(s32 *)(record + 16) += 0xffff0000;
        *(u32 *)(record + 64) = 0x80000000;
        i++;
        step -= 2;
    } while (i <= 3);
    zero = 0;
    *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)zero;
    Func_02005370(227);
    Func_02000f4c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0xffff3334,
                  0, 0xffffcccd, 0, 0);
    Func_02000f62(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0x0000cccc,
                  0, 0xffffcccd, 0, 0);
    Func_02000f86(*(s32 *)(record + 8) + 0xfffa0000, *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
    Func_02000fa2(*(s32 *)(record + 8) + (192 << 11), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
}

void Scene_DispatchApproachByFacing(void)
{
    extern u8 *Data_03001ebc;

    Func_020056c0();

    if (*(u16 *)(Func_020056ee(0) + 6) > (128 << 7)
        && *(u16 *)(Func_020056fe(0) + 6) < (192 << 8)) {
        Func_02001efc();
    } else {
        Func_02001fe2();
    }

    if (Func_020056d0(0x898) != 0) {
        Func_0200247c();
    } else {
        Func_02002350(0);
    }

    Func_0200570c();
}

void Scene_DispatchByFacing(void)
{
    extern u8 *Data_03001ebc;

    struct SceneActor_02001334 *record = GetSceneRecord(0);
    u16 angle;

    OpenSceneSection();
    SetSceneActorMode(0, 8);
    AdvanceSceneStep(20);

    angle = *(u16 *)((u8 *)record + 6);

    if ((u16)(angle - 0x2000) <= 0x3fffu) {
        RunFacingVariantA();
    } else if ((u16)(angle - 0x6000) <= 0x3fffu) {
        RunFacingVariantB();
    } else if ((u16)(angle + (192 << 7)) <= 0x3fffu) {
        RunFacingVariantC();
    } else {
        RunFacingVariantD();
    }

    SetSceneActorModeAfterBranch(0, 1);
    RunSceneVariant(1);
    CloseSceneSection();
}

void Scene_DispatchByFacingAndFlags(void)
{
    extern u8 *Data_03001ebc;

    u8 *record = Func_020057c2(0);
    u16 facing;

    Func_020057a0();

    facing = *(u16 *)(record + 6);
    if ((u16)(facing - 0x2000) <= 0x3fff) {
        Func_0200226c();
    } else if ((u16)(facing - 0x6000) <= 0x3fff) {
        Func_02001fde();
    } else if ((u16)(facing + 0x6000) <= 0x3fff) {
        Func_020021b2();
    } else {
        Func_020020d8();
    }

    Func_020058f4(0x10000, 0x2000);
    Func_02005914(20, 1);
    Func_02005910();

    if (*(s16 *)(record + 18) <= 209) {
        if (Func_020057e6(0x89a) == 0) goto scene0;
        if (Func_020057f0(0x89b) != 0) goto scene0;
        goto scene1;
scene0:
        Func_0200246a(0);
        goto firstSceneComplete;
scene1:
        Func_020025a4();
firstSceneComplete:
        Func_0200582c();
        return;
    }

    if (Func_0200580c(0x89b) != 0) {
        Func_02002486(2);
    } else if (Func_0200581e(0x89a) == 0) {
        Func_020028fe();
    } else {
        Func_0200322c();
    }
    Func_02005858();
}

void Scene_RunSecondEnsembleBeat(void)
{
    s32 id;
    u8 *rec;

    Call1_02001494(Func_080770c8, 0x89a);
    Call1_02001494(Func_0808a010, 30);
    Call3(Func_0808a148, 13, 0, 0);
    Call3(Func_0808a148, 15, 0, 0);
    Call3(Func_0808a148, 16, 0, 0);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a1e8, 13, 128 << 1, 0);
    Call3(Func_0808a1e8, 15, 128 << 1, 0);
    Call3(Func_0808a1e8, 16, 128 << 1, 0);
    Call1_02001494(Func_0808a010, 60);
    Call1_02001494(Func_0808a170, 0x183b);
    Call3(Func_0808a188, 13, 0, 20);
    Call3(Func_0808a148, 0, 13, 0);
    Call2(Func_0808a138, 15, 1);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 15, 0, 20);
    Call3(Func_0808a148, 0, 15, 0);
    Call2(Func_0808a138, 16, 2);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a148, 0, 16, 0);
    Call2(Func_0808a190, 16, 0);
    Call1_02001494(Func_0808a010, 50);
    Call2(Func_0808a200, 16, 1);
    Call3(Func_0808a090, 16, 0xcccc, 0x6666);
    Call3(Func_0808a0d0, 16, 176, 248);
    Call3(Func_0808a0d0, 16, 154 << 1, 248);
    Call3(Func_0808a1b8, 0, 128 << 6, 0);
    Call3(Func_0808a1b8, 16, 192 << 8, 20);
    Call1_02001494(Func_080f9010, 158);
    Call3(Func_08009178, 0x0200c77a, 78, 13);
    Call2(Func_0808a138, 16, 2);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a090, 16, 192 << 9, 192 << 8);
    rec = Record1(Func_0808a080, 16);
    rec[90] &= 0xfe;
    Call3(Func_0808a0d0, 16, 154 << 1, 136 << 1);
    Call1_02001494(Func_0808a010, 1);
    rec = Record1(Func_0808a080, 16);
    {
        /*
         * A result temporary, not the compound or-assign the matching
         * &= 0xfe case above uses. The reference writes the result into
         * the mask register rather than the loaded value, and the
         * two-address ORR only does that when the merged result is its
         * own object; the compound form keeps the loaded value as
         * destination. Same technique already adopted in the sibling
         * owner resource_3bd:020013f8.
         */
        u8 merged = (u8)(rec[90] | 1);

        rec[90] = merged;
    }
    Call3(Func_0808a188, 16, 0, 50);
    Call3(Func_0808a0f0, 17, 152 << 17, 216 << 16);
    Call3(Func_0808a0d0, 17, 152 << 1, 248);
    Call3(Func_0808a148, 9, 17, 0);
    Call3(Func_0808a148, 10, 17, 0);
    Call3(Func_0808a148, 11, 17, 0);
    Call3(Func_0808a148, 12, 17, 0);
    Call3(Func_0808a148, 13, 17, 0);
    Call3(Func_0808a148, 14, 17, 0);
    Call3(Func_0808a148, 15, 17, 0);
    Call3(Func_0808a148, 16, 17, 0);
    Call3(Func_0808a148, 0, 17, 0);
    Call1_02001494(Func_0808a010, 10);
    Call2(Func_0808a130, 9, 2);
    Call2(Func_0808a130, 10, 2);
    Call2(Func_0808a130, 11, 2);
    Call2(Func_0808a130, 12, 2);
    Call2(Func_0808a130, 13, 2);
    Call2(Func_0808a130, 14, 2);
    Call2(Func_0808a130, 15, 2);
    Call2(Func_0808a138, 16, 2);
    Call3(Func_0808a1e8, 17, 0x103, 60);
    Call3(Func_0808a0f0, 18, 152 << 17, 216 << 16);
    Call3(Func_0808a0c8, 18, 152 << 1, 248);
    Call3(Func_0808a0c8, 17, 140 << 1, 132 << 1);
    Call1_02001494(Func_0808a0e8, 18);
    Call3(Func_0808a1b8, 18, 160 << 7, 0);
    Call1_02001494(Func_0808a0e8, 17);
    Call1_02001494(Func_080f9010, 159);
    Call3(Func_08009178, 0x0200c790, 78, 13);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a148, 9, 17, 0);
    Call3(Func_0808a148, 10, 17, 0);
    Call3(Func_0808a148, 11, 17, 0);
    Call3(Func_0808a148, 12, 17, 0);
    Call3(Func_0808a148, 13, 17, 0);
    Call3(Func_0808a148, 14, 17, 0);
    Call3(Func_0808a148, 15, 17, 0);
    Call3(Func_0808a148, 16, 17, 0);
    Call3(Func_0808a148, 0, 17, 0);
    Call1_02001494(Func_0808a010, 10);
    Call2(Func_0808a138, 17, 2);
    Call1_02001494(Func_0808a010, 20);
    Call2(Func_0808a110, 18, 4);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 17, 3);
    Call1_02001494(Func_0808a010, 20);
    Call2(Func_0808a138, 18, 1);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a1b8, 17, 208 << 8, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call3(Func_0808a1e8, 18, 0x102, 60);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 17, 3);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a110, 18, 3);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 17, 3);
    Call1_02001494(Func_0808a010, 20);
    Call2(Func_0808a110, 18, 4);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a138, 17, 2);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a1b8, 17, 0, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call3(Func_0808a1b8, 16, 128 << 8, 20);
    Call2(Func_0808a110, 16, 3);
    Call1_02001494(Func_0808a010, 20);
    Call2(Func_0808a110, 17, 3);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a110, 16, 3);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 16, 0, 20);
    Call3(Func_0808a1b8, 17, 128 << 8, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a110, 9, 3);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 9, 0, 20);
    Call3(Func_0808a1b8, 17, 208 << 8, 20);
    Call2(Func_0808a138, 17, 1);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call3(Func_0808a1e8, 18, 0x102, 60);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a1e8, 17, 0x101, 60);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a110, 18, 3);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a1e8, 17, 0x100, 60);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a110, 18, 4);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a1e8, 17, 0x103, 60);
    Call3(Func_0808a188, 17, 0, 20);
    Call3(Func_0808a1e8, 18, 0x100, 60);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 17, 4);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a138, 18, 2);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a138, 17, 2);
    Call1_02001494(Func_0808a010, 10);
    Call3(Func_0808a0d0, 17, 128 << 1, 140 << 1);
    Call3(Func_0808a1b8, 17, 128 << 7, 20);
    Call3(Func_0808a0f0, 17, 0, 0);
    Call1_02001494(Func_0808a088, 17);
    Call1_02001494(Func_0808a010, 30);
    Call2(Func_0808a138, 9, 2);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 9, 0, 20);
    Call2(Func_0808a138, 15, 2);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 15, 0, 20);
    Call3(Func_0808a148, 16, 18, 0);
    Call1_02001494(Func_0808a010, 20);
    Call2(Func_0808a138, 16, 2);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 16, 0, 20);
    Call3(Func_0808a148, 18, 16, 0);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 18, 4);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 18, 3);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a090, 18, 0xcccc, 0x6666);
    Call3(Func_0808a0d0, 18, 128 << 1, 248);
    Call3(Func_0808a1b8, 18, 192 << 8, 20);
    Call2(Func_0808a130, 18, 1);
    Call3(Func_0808a1e8, 18, 0x100, 60);
    Call3(Func_0808a0d0, 18, 240, 184);
    Call2(Func_0808a138, 18, 2);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a0f0, 19, 232 << 16, 168 << 16);
    Call3(Func_0808a0f0, 20, 232 << 16, 168 << 16);
    rec = Record1(Func_0808a080, 19);
    *(s32 *)(rec + 12) = 0xc0000;
    rec = Record1(Func_0808a080, 19);
    *(s32 *)(rec + 60) = -0x80000000;
    rec = Record1(Func_0808a080, 19);
    *(s32 *)(rec + 24) = 0xcccc;
    rec = Record1(Func_0808a080, 19);
    {
        u8 *target = *(u8 **)(rec + 80);
        s32 shown = 0x8000;

        *(u16 *)(target + 30) = shown;
    }
    Call1_02001494(Func_080f9010, 124);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a1b8, 0, 192 << 8, 20);
    Call3(Func_0808a0d0, 16, 128 << 1, 240);
    Call3(Func_0808a1b8, 16, 176 << 8, 20);
    Call2(Func_0808a138, 16, 1);
    Call3(Func_0808a188, 16, 0, 20);
    Call3(Func_0808a148, 9, 0, 0);
    Call3(Func_0808a148, 10, 0, 0);
    Call3(Func_0808a148, 11, 0, 0);
    Call3(Func_0808a148, 12, 0, 0);
    Call3(Func_0808a148, 13, 0, 0);
    Call3(Func_0808a148, 14, 0, 0);
    Call3(Func_0808a148, 15, 0, 0);
    Call3(Func_0808a148, 16, 0, 0);
    Call2(Func_0808a138, 18, 2);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a1b8, 18, 160 << 7, 20);
    Call3(Func_0808a0d0, 18, 248, 208);
    Call3(Func_0808a1b8, 18, 160 << 7, 20);
    Value2(Func_0808a178, 18, 0);
    if (Value2(Func_0808a070, 0, 0) == 0) {
        Value2((s32 (*)())Func_0808a138, 16, 1);
        Call1_02001494(Func_0808a010, 20);
        id = 16;
        goto joinBeat;
    }

    /* Skipped once: bump the workspace skip counter and offer the beat again. */
    Scene_BumpStep(1);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a1e8, 18, 0x105, 60);
    Call3(Func_0808a1b8, 18, 128 << 7, 20);
    Call2(Func_0808a138, 16, 2);
    Call1_02001494(Func_0808a010, 20);
    Value2(Func_0808a178, 16, 0);
    if (Value2(Func_0808a070, 0, 0) != 0) {
        goto skipTwice;
    }
    Call2(Func_0808a110, 16, 3);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a1b8, 18, 176 << 8, 20);
    id = 18;

joinBeat:
    Call3(Func_0808a188, id, 0, 20);
    Call1_02001494(Func_080770c8, 0x898);
    goto finish;

skipTwice:
    Scene_BumpStep(1);
    Call1_02001494(Func_0808a010, 20);
    Call2(Func_0808a110, 18, 4);
    Call1_02001494(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call1_02001494(Func_080770c8, 0x899);

finish:
    Call3(Func_0808a1b8, 10, 128 << 8, 0);
    Call3(Func_0808a1b8, 11, 128 << 8, 20);
    Call2(Func_0808a100, 10, 5);
    Call2(Func_0808a100, 11, 5);
    Func_0808a098(12, 0x0200c638);
}

void Scene_RunSkippableStoryBeat(void)
{
    extern u8 *Data_03001ebc;

    u8 *workspace;

    Func_02006130();
    Func_020061f6(0x186e);
    Func_02006206(18, 0);

    if (Func_0200615e(0, 0) == 0) {
        Func_02006148(20);
        Func_02006232(18, 0, 20);
        Func_02006148_a(0x898);
        Func_0200616c();
    } else {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
        Func_02006258(18, 0, 20);
        Func_0200618c();
    }
}

/* Looks up actors 18, 13, 14, 15 and 16 and clears their +108 field before
 * the scene runs. */
void Scene_RunEnsembleStoryBeat(void)
{
    extern u8 Data_03001ebc[];

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
    Motion_SetSpeedLim_1(0x8000, 0x1000);
    Motion_CamBounds_1(0xe80000, -1, 0xc80000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    SceneWork_SetStepValue_1(0x1883);
    Motion_SetSpeed_1(10, 0xcccc, 0x6666);
    Motion_SetSpeed_2(12, 0xcccc, 0x6666);
    Motion_ResetPosMode2_1(10, 152, 200);
    Motion_SetPosReset_1(12, 144, 248);
    Motion_CommitPos_1(10);
    Motion_SetAngleToward_1(9, 19, 0);
    Motion_SetAngleToward_2(11, 19, 0);
    Motion_SetAngleToward_3(13, 19, 0);
    Motion_SetAngleToward_4(14, 19, 0);
    Motion_SetAngleToward_5(15, 19, 0);
    Motion_SetAngleToward_6(16, 19, 0);
    Motion_SetAngleToward_7(18, 19, 0);
    Motion_SetSpeed_3(10, 0x18000, 0xc000);
    Motion_SetSpeed_4(12, 0x20000, 0x10000);
    Motion_ResetPosMode2_2(10, 152, 200);
    Motion_SetPosReset_2(12, 144, 248);
    Motion_SetAngleToward_8(12, 19, 0);
    Motion_CommitPos_2(10);
    Motion_SetAngleToward_9(10, 19, 0);
    Motion_SetVarCbObj_1(18, 2);
    Battle_WaitMode0_1(20);
    Battle_RunThenWaitIfModeZero_1(18, 0, 40);
    Motion_SetAngleToward_10(9, 18, 0);
    Motion_SetAngleToward_11(10, 18, 0);
    Motion_ArmCb_1(11, 0x3000, 0);
    Motion_SetAngleToward_12(12, 18, 0);
    Motion_ArmCb_2(13, 0x3000, 0);
    Motion_SetAngleToward_13(14, 18, 0);
    Motion_SetAngleToward_14(15, 18, 0);
    Motion_SetAngleToward_15(16, 18, 0);
    Battle_WaitMode0_2(20);
    Battle_RunThenWaitIfModeZero_2(16, 0, 20);
    Motion_CallWaitAnim_1(18, 3);
    Battle_WaitMode0_3(20);
    Motion_CallWaitAnim_2(16, 3);
    Battle_WaitMode0_4(20);
    Battle_RunThenWaitIfModeZero_3(16, 0, 20);
    BattleFx_SpawnLinked_1(18, 0x105, 60);
    BattleFx_SpawnLinked_2(16, 0x101, 60);
    Battle_RunThenWaitIfModeZero_4(16, 0, 20);
    Battle_RunThenWaitIfModeZero_5(18, 0, 20);
    BattleFx_SpawnLinked_3(16, 0x102, 60);
    BattleFx_SpawnLinked_4(15, 0x101, 60);
    Motion_SetSpeed_5(15, 0xcccc, 0x6666);
    Motion_SetPosReset_3(15, 216, 176);
    Motion_ArmCb_3(15, 0x3000, 20);
    Battle_RunThenWaitIfModeZero_6(15, 0, 20);
    Motion_ArmCb_4(18, 0xb000, 20);
    Motion_CallWaitAnim_3(18, 4);
    Battle_WaitMode0_5(20);
    Battle_RunThenWaitIfModeZero_7(18, 0, 20);
    Motion_SetVarCb_1(9, 2);
    Motion_SetVarCb_2(10, 2);
    Motion_SetVarCb_3(11, 2);
    Motion_SetVarCb_4(12, 2);
    Motion_SetVarCb_5(13, 2);
    Motion_SetVarCb_6(14, 2);
    Motion_SetVarCb_7(15, 2);
    Motion_SetVarCb_8(16, 2);
    Battle_WaitMode0_6(40);
    Motion_SetVarCbObj_2(13, 2);
    Battle_RunThenWaitIfModeZero_8(13, 0, 20);
    Motion_CallWaitAnim_4(18, 3);
    Battle_WaitMode0_7(20);
    Battle_RunThenWaitIfModeZero_9(18, 0, 20);
    Motion_ArmCb_5(0, 0xe000, 20);
    Motion_ArmCb_6(18, 0x5000, 20);
    BattleEventRuntime_ProcessAction_1(18, 0);
    BattleFx_SpawnLinked_5(9, 0x101, 0);
    Battle_WaitMode0_8(5);
    BattleFx_SpawnLinked_6(10, 0x101, 0);
    Battle_WaitMode0_9(5);
    BattleFx_SpawnLinked_7(11, 0x101, 0);
    Battle_WaitMode0_10(5);
    BattleFx_SpawnLinked_8(12, 0x101, 0);
    Battle_WaitMode0_11(5);
    BattleFx_SpawnLinked_9(13, 0x101, 0);
    Battle_WaitMode0_12(5);
    BattleFx_SpawnLinked_10(14, 0x101, 0);
    Battle_WaitMode0_13(5);
    BattleFx_SpawnLinked_11(15, 0x101, 0);
    Battle_WaitMode0_14(5);
    BattleFx_SpawnLinked_12(16, 0x101, 0);
    Battle_WaitMode0_15(60);
    Motion_SetVarCbObj_3(16, 2);
    Battle_RunThenWaitIfModeZero_10(16, 0, 20);
    Motion_CallWaitAnim_5(18, 3);
    Battle_WaitMode0_16(20);
    Battle_RunThenWaitIfModeZero_11(18, 0, 20);
    BattleFx_SpawnLinked_13(15, 0x101, 60);
    Battle_RunThenWaitIfModeZero_12(15, 0, 20);
    Motion_SetAngleToward_16(18, 15, 0);
    Battle_WaitMode0_17(20);
    Motion_CallWaitAnim_6(18, 4);
    Battle_WaitMode0_18(20);
    Battle_RunThenWaitIfModeZero_13(18, 0, 40);
    Object_LinkPair_1(11, 10, 0);
    Object_LinkPair_2(12, 14, 0);
    Object_LinkPair_3(13, 15, 0);
    Battle_WaitMode0_19(60);
    Motion_SetAngleToward_17(10, 18, 0);
    Motion_SetAngleToward_18(11, 18, 0);
    Motion_SetAngleToward_19(12, 18, 0);
    Motion_SetAngleToward_20(13, 18, 0);
    Motion_SetAngleToward_21(14, 18, 0);
    Motion_SetAngleToward_22(15, 18, 0);
    Battle_WaitMode0_20(20);
    Battle_RunThenWaitIfModeZero_14(18, 0, 20);
    Motion_SetVarCbObj_4(18, 2);
    Battle_WaitMode0_21(20);
    Object_SetModeById_2(9, 3);
    Object_SetModeById_3(10, 3);
    Object_SetModeById_4(11, 3);
    Object_SetModeById_5(12, 3);
    Object_SetModeById_6(13, 3);
    Object_SetModeById_7(14, 3);
    Object_SetModeById_8(15, 3);
    Motion_CallWaitAnim_7(16, 3);
    Battle_WaitMode0_22(20);
    Motion_ArmCb_7(18, 0x5000, 20);
    BattleEventRuntime_ProcessAction_2(18, 0);
    Motion_CallWaitAnim_8(18, 3);
    Battle_WaitMode0_23(20);
    Battle_RunThenWaitIfModeZero_15(18, 0, 20);
    Object_SetModeById_9(0, 3);
    Object_SetModeById_10(9, 3);
    Object_SetModeById_11(10, 3);
    Object_SetModeById_12(11, 3);
    Object_SetModeById_13(12, 3);
    Object_SetModeById_14(13, 3);
    Object_SetModeById_15(14, 3);
    Object_SetModeById_16(15, 3);
    Motion_CallWaitAnim_9(16, 3);
    Battle_WaitMode0_24(20);
    Motion_SetVarCbObj_5(18, 2);
    Battle_WaitMode0_25(20);
    Battle_RunThenWaitIfModeZero_16(18, 0, 20);
    Battle_RunThenWaitIfModeZero_17(18, 0, 20);
    Object_SetModeById_17(0, 3);
    Motion_CallWaitAnim_10(18, 3);
    Battle_WaitMode0_26(20);
    Motion_ArmCb_8(18, 0x8000, 20);
    Motion_SetVarCbObj_6(18, 2);
    Battle_WaitMode0_27(20);
    Battle_RunThenWaitIfModeZero_18(18, 0, 20);
    Object_SetModeById_18(9, 3);
    Object_SetModeById_19(10, 3);
    Object_SetModeById_20(11, 3);
    Object_SetModeById_21(12, 3);
    Object_SetModeById_22(13, 3);
    Object_SetModeById_23(14, 3);
    Object_SetModeById_24(15, 3);
    Motion_CallWaitAnim_11(16, 3);
    Battle_WaitMode0_28(20);
    Motion_ResetPosMode2_3(10, 120, 200);
    Motion_ResetPosMode2_4(12, 120, 248);
    Motion_CommitPos_3(10);
    Motion_ArmCb_9(11, 0x8000, 20);
    Object_SetModeById_25(10, 5);
    Object_SetModeById_26(11, 5);
    Motion_CommitPos_4(12);
    Motion_EnableActCb_1(12, 0x200c638);
    Motion_SetSpeed_6(15, 0xcccc, 0x6666);
    Motion_SetPosReset_4(15, 216, 168);
    Motion_SetPosReset_5(15, 232, 168);
    Motion_ArmCb_10(15, 0xc000, 20);
    Motion_SetVarCbObj_7(15, 3);
    Motion_SetHPosTerrain_1(19, 0xe80000, 0xa80000);
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
    Battle_WaitMode0_29(40);
    Motion_SetPosReset_6(15, 216, 152);
    Motion_ArmCb_11(15, 0x4000, 30);
    GameFlag_Clear_1(0x898);
    GameFlag_Set_1(0x89b);
}

void Scene_ShowDialogue1A58(void)
{
    extern u8 *Data_03001ebc;

    Func_02006844();
    Func_0200690a(0x1a58);
    Func_02006932(11, 0);
    Func_0200685e();
}

void StartSchoolDoorEvent(void)
{
    Func_02006864();
    if (Func_0200684a(2202) == 0 && Func_02006854(2197) == 0) {
        Func_02006858(6317, 1);
        Func_0200688c();
    } else {
        Func_02006a2c(158);
        Func_0200684e(0x0200c77a, 78, 13);
        SetScale(0, 0x8000, 0x4000);
        SetPosition(0, 306, 248);
        Func_0200690c(0, 304, 216);
        Func_020068ba(20);
        Func_020069f0(4);
        Func_020068d4();
    }
}

void Scene_DispatchByRange(void)
{
    extern u8 *Data_03001ebc;

    u8 *record;
    u32 biased;

    record = Func_02006912(0);
    biased = *(u16 *)(record + 6);
    Func_020068f0();

    biased = biased + 0xffff5fff;
    if (biased <= 0x3ffe) {
        Func_02006a98(13);
    } else {
        Func_020069c8(0x1a1c);
        Func_020069e0(13, 0);
    }

    Func_0200691c();
}

void Scene_ShowDialogue17DF(void)
{
    extern u8 *Data_03001ebc;

    Func_0200692c();
    Func_020069cc(8, 2);
    Func_020069fa(0x17df);
    Func_02006a12(8, 0);
    Func_0200694e();
}

s32 Scene_SelectData(void)
{
    extern u8 *Data_03001ebc;

    if (Data_02000240[224] == (s32)&Value_0000003c) {
        return (s32)Data_0200cb90;
    }
    if (Data_02000240[225] == 3) {
        return (s32)Data_0200d184;
    }
    return (s32)Data_0200cd40;
}

void Scene_SpawnRandomizedParticle(void)
{
    extern u8 *Data_03001ebc;

    struct Params params;
    u8 *record;
    s32 draw;
    s32 offset;

    record = Func_020069c4(0);

    params.field1 = 7;
    draw = (u32)(Func_02006928() * 7) >> 16;
    if ((draw & 7) == 0)
        params.field1 = 5;

    params.field2 = 0xb333;
    params.field3 = 0xcccc;

    offset = ((u32)(Func_02006944() * 8) >> 16) * 13107;

    Func_02002766(*(s32 *)(record + 8) + ((8 - (Data_03001e40 & 15)) << 16),
                  *(s32 *)(record + 12) + (192 << 13),
                  *(s32 *)(record + 16),
                  0,
                  -offset,
                  0,
                  144 << 12,
                  (u8 *)&params);

    if ((Data_03001e40 & 1) != 0)
        Func_02006ac8(0, 15);
    else
        Func_02006ad2(0, 1);
}

void Scene_ApplyOffset0Neg32(void)
{
    extern u8 *Data_03001ebc;

    Func_02004cf2(0, -32);
}

void Scene_ApplyOffset0Pos32(void)
{
    extern u8 *Data_03001ebc;

    Func_02004d00(0, 32);
}

void Scene_ApplyOffsetNeg32_0(void)
{
    extern u8 *Data_03001ebc;

    Func_02004d12(-32, 0);
}

void Scene_RunOpeningAuxiliarySequence(s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02006a70();
    Call3_0200268c(Func_02006ab6, 0, 0x28000, 0x14000);
    Value3(Func_02006ae8, 0, a0, a1);
    Func_02006b1a(0, 4, 0);
    Func_02006b12(0, 7);
    Func_02006b08(0);
    Func_02006b20(0, 6);
    Func_02006ab4();
}

void Scene_RunScene39eSequenceA(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_200a5b9;

    Func_02006ab8();
    base5_200a5b9 = (s32)Func_0200a5b9;
    Call2_020026d8(Func_02006a34, base5_200a5b9, 0xc80);
    Call3(Func_02006b06, 0, 0x3333, 0x1999);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 60;
    Func_02006c30();
    Func_02006c86(154);
    Func_02006b66(0, 2);
    Call3(Func_02006b5a, 0, 0, -6);
    Func_02006b68(0);
    Func_02006bb8(0, 15);
    record = Func_02006b36(0);
    Func_02006adc(record, 0);
    Func_02006a92(base5_200a5b9);
    Func_02006c76();
    Func_02006c4c(3);
    Func_02006b30();
}

void Scene_PlaySound123AndEnable(void)
{
    extern u8 *Data_03001ebc;

    Func_02006ce6(123);
    Func_02006c74(1);
}

void FieldScene_RunScene39e_02002778(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02006b58();
    Func_02006cfe(188);
    Call3_02002778(Func_02006b20_a, 0x200c764, 77, 8);
    *(u8 *)(Func_02006b96(0) + 85) = 0;
    Call3_02002778(Func_02006bb6, 0, 0xcccc, 0x6666);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Func_02006c0c(0, 2);
    Call3_02002778(Func_02006c00, 0, 0, -16);
    Func_02006b9e(16);
    Func_02006cd4(2);
    Func_02006bb8_a();
}

void FieldScene_RunScene39e_020027ec(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02006bcc();
    Call3_020027ec(Func_02006c12, 0, 0x8000, 0x4000);
    Call3_020027ec(Func_02006c36, 0, 168, 0x1f8);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Func_02006d40();
    Func_02006d54();
    Func_02006c6a(0);
    Func_02006c00_a(20);
    Func_02006ca0(8, 2);
    Call2(Func_02006d12, 8, 0x102);
    Func_02006c18(60);
    *(u8 *)(Func_02006c4e(8) + 91) = 0;
    Func_02006dd2(152);
    record = Func_02006c60(8);
    *(volatile s32 *)(record + 40) = 0x80000;
    Func_02006cbe(8, 1);
    Call1_02002778(Func_02006d0c, 0x17be);
    Func_02006d2e(8, 0, 20);
    Func_02006d38(8, 0, 20);
    Func_02006ce8(0, 3);
    Func_02006c66(20);
    Func_02006cf6(8, 3);
    Func_02006c74_a(20);
    Func_02006d5e(8, 0, 20);
    Call3_020027ec(Func_02006d80, 0, 0x101, 60);
    Value2(Func_02006d60, 8, 0);
    if (Value2(Func_02006cb8, 0, 0) == 0) {
        Func_02006ca2(10);
        Func_02006d32(8, 3);
        Func_02006cb0(20);
        Func_02006d9a(8, 0, 20);
        bump_step(2);
    } else {
        Func_02006cd0(10);
        Func_02006d78(8, 2);
        Func_02006cde(20);
        bump_step(1);
        Func_02006dd6(8, 0, 20);
        Func_02006d86(8, 3);
        Func_02006d04(20);
        Func_02006dee(8, 0, 20);
    }
    Func_02006df8(8, 0, 20);
    Func_02006dc0(8, 2);
    Func_02006d26(20);
    Func_02006e10(8, 0, 20);
    Call3_020027ec(Func_02006e32, 0, 0x101, 60);
    Value2(Func_02006e12, 8, 0);
    if (Value2(Func_02006d6a, 0, 0) == 1) {
        Func_02006d54_a(10);
        Call3_020027ec(Func_02006e58, 8, 0x102, 60);
        Call1_02002778(Func_02006e2e, 0x17c8);
        Func_02006e3e(8, 0);
        L_0200299c:;
        if (Value2(Func_02006d96, 0, 0) == 1) {
            Func_02006d80_a(10);
            Call3_020027ec(Func_02006e84, 8, 0x102, 60);
            Func_02006e5a((s32)Data_000017e0);
            Func_02006e6a(8, 0);
            goto L_0200299c;
        }
    }
    Call1_02002778(Func_02006e6a_a, 0x17c9);
    Func_02006da8(10);
    Func_02006e38(8, 3);
    Func_02006db6(20);
    Value2(Func_02006e8e, 8, 0);
    if (Value2(Func_02006de6, 0, 0) == 0) {
        Func_02006dd0(10);
        Func_02006e60(0, 3);
        Func_02006dde(20);
        Func_02006ec8(8, 0, 20);
        bump_step(1);
    } else {
        Func_02006e00(10);
        Func_02006ea8(8, 2);
        bump_step(1);
        Func_02006f02(8, 0, 20);
    }
    Func_02006eb2(8, 3);
    Func_02006e30(20);
    Func_02006f1a(8, 0, 20);
    Func_02006ee2(8, 2);
    Func_02006e48(20);
    Func_02006f32(8, 0, 20);
    Func_02006ee2_a(0, 3);
    Func_02006e60_a(20);
    Func_02006ef0(8, 3);
    Func_02006e6e(20);
    Func_02006ef6(8, 5);
    Call1_02002778(Func_02006e6c, 0x893);
    Func_02006e90();
}

/* Long scripted sequence: sets up and steps a series of actors (indices 0-3,
 * 8-10, 17, 20) through position, pose, animation, wait, and flag-bit calls,
 * copying a couple of fields between some actors' records along the way. */
void Scene_RunRoofEnsembleSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 unused;
    u8 *source_record;

    Battle_Reset_1();
    ObjectMotion_SetSpeedParameters_1_02002778(0, 0x6666, 0x3333);
    ObjectMotion_ResetAndSetPositionInMode2_1_02002778(0, 0x1d8, 0x218);
    BattleRuntime_WaitIfModeZero_1_02002778();
    ObjectMotion_SetSpeedParameters_2_02002778();
    ObjectMotion_CommitCurrentPositionAndActivate_1_02002778(0);
    ObjectMotion_ArmCallback_1_02002778(9, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02002778(9, 2);
    BattleRuntime_WaitIfModeZero_2_02002778(20);
    SceneWork_SetStepValue_1_02002778(0x1969);
    BattleRuntime_RunThenWaitIfModeZero_1_02002778(9, 0, 20);
    ObjectMotion_ArmCallback_2_02002778(0, 0x8000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_1_02002778(0, 3);
    BattleRuntime_WaitIfModeZero_3_02002778(30);
    ObjectMotion_ArmCallback_3_02002778(8, 0x4000, 30);
    ObjectMotion_CallThenWaitForAnimationChange_2_02002778(8, 3);
    BattleRuntime_WaitIfModeZero_4_02002778(20);
    BattleRuntime_RunThenWaitIfModeZero_2_02002778(8, 0, 20);
    ObjectMotion_ArmCallback_4_02002778(0, 0xc000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2_02002778(0, 1);
    BattleRuntime_WaitIfModeZero_5_02002778(20);
    ObjectMotion_ArmCallback_5_02002778(9, 0xd000, 0);
    /* Copy the pair of s32 fields at +8/+16 from actor 0's record (as seen
     * through each accessor) into the matching setter for another actor. */
    source_record = Scene_GetRecord_1_02002778(0);
    if (source_record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_1_02002778(1, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    source_record = Scene_GetRecord_2_02002778(0);
    if (source_record != 0) {
        Motion_SetHPosTerrain_2(2, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    source_record = Scene_GetRecord_3_02002778(0);
    if (source_record != 0) {
        Motion_SetHPosTerrain_3(3, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    ObjectMotion_SetSpeedParameters_3_02002778(0, 0x8000, 0x4000);
    ObjectMotion_SetSpeedParameters_4_02002778(1, 0x8000, 0x4000);
    ObjectMotion_SetSpeedParameters_5_02002778(2, 0x8000, 0x4000);
    ObjectMotion_SetSpeedParameters_6_02002778(3, 0x8000, 0x4000);
    ObjectMotion_ResetAndSetPositionInMode2_2_02002778(0, 0x1d0, 0x1f8);
    ObjectMotion_ResetAndSetPositionInMode2_3_02002778(2, 0x1e0, 0x1f8);
    ObjectMotion_ResetAndSetPositionInMode2_4_02002778(1, 0x1f0, 0x1f0);
    Motion_ResetPosMode2_5(3, 0x1c0, 0x1f0);
    ObjectMotion_CommitCurrentPositionAndActivate_2_02002778(0);
    ObjectMotion_CommitCurrentPositionAndActivate_3_02002778(2);
    ObjectMotion_CommitCurrentPositionAndActivate_4_02002778(3);
    ObjectMotion_ArmCallback_6_02002778(3, 0xe000, 0);
    Motion_CommitPos_5(1);
    ObjectMotion_ArmCallback_7_02002778(1, 0xa000, 0);
    BattleRuntime_WaitIfModeZero_6_02002778(20);
    BattleEffect_SpawnLinkedResourceObject_1_02002778(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_2_02002778(1, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_3_02002778(2, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_4_02002778(3, 0x101, 60);
    ObjectMotion_ArmCallback_8_02002778(1, 0x5000, 20);
    BattleEventRuntime_ProcessAction_1_02002778(1, 0);
    BattleRuntime_WaitIfModeZero_7_02002778(20);
    ObjectMotion_CallThenWaitForAnimationChange_3_02002778(8, 3);
    BattleRuntime_WaitIfModeZero_8_02002778(20);
    BattleRuntime_RunThenWaitIfModeZero_3_02002778(8, 0, 20);
    ObjectMotion_ArmCallback_9_02002778(1, 0xa000, 20);
    BattleEffect_SpawnLinkedResourceObject_5_02002778(3, 0x100, 60);
    BattleRuntime_RunThenWaitIfModeZero_4_02002778(3, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_4_02002778(8, 3);
    BattleRuntime_WaitIfModeZero_9_02002778(20);
    BattleRuntime_RunThenWaitIfModeZero_5_02002778(8, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_6_02002778(2, 0x100, 60);
    BattleRuntime_RunThenWaitIfModeZero_6_02002778(2, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_5_02002778(8, 3);
    ObjectMotion_ArmCallback_10_02002778(8, 0x3000, 20);
    BattleRuntime_RunThenWaitIfModeZero_7_02002778(8, 0, 20);
    ObjectMotion_ArmCallback_11_02002778(0, 0, 0);
    Motion_ArmCb_12(1, 0x6000, 0);
    Motion_ArmCb_13(3, 0x2000, 0);
    ObjectMotion_SetVariantCallback_1_02002778(2, 2);
    BattleRuntime_WaitIfModeZero_10_02002778(2, 0x102);
    BattleRuntime_WaitIfModeZero_11_02002778(60);
    Motion_ArmCb_14(0, 0xc000, 0);
    Motion_ArmCb_15(1, 0xa000, 0);
    Motion_ArmCb_16(3, 0xe000, 0);
    BattleRuntime_WaitIfModeZero_12_02002778(20);
    BattleRuntime_RunThenWaitIfModeZero_8_02002778(2, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_6_02002778(8, 3);
    BattleRuntime_WaitIfModeZero_13_02002778(20);
    BattleRuntime_RunThenWaitIfModeZero_9_02002778(8, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3_02002778(2, 2);
    BattleRuntime_WaitIfModeZero_14_02002778(20);
    BattleRuntime_RunThenWaitIfModeZero_10_02002778(2, 0, 20);
    BattleRuntime_RunThenWaitIfModeZero_11_02002778(8, 0, 20);
    Object_LinkPair_1_02002778(0, 2, 50);
    Motion_ArmCb_17(0, 0xc000, 0);
    Motion_ArmCb_18(2, 0xc000, 30);
    ObjectMotion_CallThenWaitForAnimationChange_7_02002778(2, 3);
    BattleRuntime_WaitIfModeZero_15_02002778(20);
    BattleRuntime_RunThenWaitIfModeZero_12_02002778(8, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_7_02002778(3, 0x101, 60);
    BattleRuntime_RunThenWaitIfModeZero_13_02002778(3, 0, 20);
    Motion_ArmCb_19(8, 0x5000, 20);
    BattleRuntime_RunThenWaitIfModeZero_14_02002778(8, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4_02002778(3, 1);
    BattleRuntime_WaitIfModeZero_16_02002778(20);
    ObjectMotion_CallThenWaitForAnimationChange_8_02002778(8, 3);
    BattleRuntime_WaitIfModeZero_17_02002778(20);
    BattleRuntime_RunThenWaitIfModeZero_15_02002778(8, 0, 20);
    Motion_ArmCb_20(0, 0xe000, 0);
    Motion_ArmCb_21(1, 0x6000, 0);
    BattleRuntime_WaitIfModeZero_18_02002778(30);
    Motion_ArmCb_22(0, 0xc000, 0);
    Motion_ArmCb_23(1, 0xa000, 20);
    BattleRuntime_RunThenWaitIfModeZero_16_02002778(1, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_9_02002778(8, 4);
    BattleRuntime_WaitIfModeZero_19_02002778(20);
    ObjectMotion_SetAngleToward_1_02002778(8, 2, 0);
    BattleRuntime_WaitIfModeZero_20_02002778(10);
    BattleRuntime_RunThenWaitIfModeZero_17_02002778(8, 0, 20);
    ObjectMotion_SetAngleToward_2_02002778(0, 2, 0);
    ObjectMotion_SetAngleToward_3_02002778(1, 2, 0);
    ObjectMotion_SetAngleToward_4_02002778(3, 2, 0);
    BattleEffect_SpawnLinkedResourceObject_8_02002778(2, 0x102, 60);
    BattleRuntime_RunThenWaitIfModeZero_18_02002778(2, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_10_02002778(8, 3);
    BattleRuntime_WaitIfModeZero_21_02002778(20);
    Battle_RunThenWaitIfModeZero_19(8, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_11_02002778(2, 3);
    BattleRuntime_WaitIfModeZero_22_02002778(20);
    ObjectMotion_SetVariantCallback_2_02002778(1, 2);
    BattleRuntime_WaitIfModeZero_23_02002778(20);
    Battle_RunThenWaitIfModeZero_20(1, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_9_02002778(8, 0x100, 60);
    ObjectMotion_SetAngleToward_5_02002778(8, 1, 0);
    BattleRuntime_WaitIfModeZero_24_02002778(20);
    Battle_RunThenWaitIfModeZero_21(8, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_10_02002778(2, 0x100, 60);
    Battle_RunThenWaitIfModeZero_22(2, 0, 20);
    ObjectMotion_SetAngleToward_6_02002778(8, 1, 0);
    Motion_ArmCb_24(0, 0xc000, 0);
    Motion_ArmCb_25(1, 0xa000, 0);
    Motion_ArmCb_26(3, 0xe000, 0);
    BattleRuntime_WaitIfModeZero_25_02002778(20);
    Motion_CallWaitAnim_12(8, 3);
    BattleRuntime_WaitIfModeZero_26_02002778(20);
    Battle_RunThenWaitIfModeZero_23(8, 0, 20);
    ObjectMotion_SetVariantCallback_3_02002778(2, 2);
    BattleRuntime_WaitIfModeZero_27_02002778(20);
    Battle_RunThenWaitIfModeZero_24(2, 0, 20);
    Motion_CallWaitAnim_13(8, 3);
    BattleRuntime_WaitIfModeZero_28_02002778(20);
    Battle_RunThenWaitIfModeZero_25(8, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5_02002778(3, 2);
    BattleRuntime_WaitIfModeZero_29_02002778(20);
    Battle_RunThenWaitIfModeZero_26(3, 0, 20);
    Motion_ArmCb_27(8, 0x5000, 20);
    Motion_CallWaitAnim_14(8, 3);
    Battle_WaitMode0_30(20);
    Battle_RunThenWaitIfModeZero_27(8, 0, 20);
    ObjectMotion_SetVariantCallback_4_02002778(0, 2);
    ObjectMotion_SetVariantCallback_5_02002778(1, 2);
    ObjectMotion_SetVariantCallback_6_02002778(2, 2);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6_02002778(3, 2);
    Battle_WaitMode0_31(20);
    Motion_CallWaitAnim_15(8, 4);
    Battle_WaitMode0_32(20);
    Battle_RunThenWaitIfModeZero_28(8, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7_02002778(8, 2);
    Battle_WaitMode0_33(20);
    Battle_RunThenWaitIfModeZero_29(8, 0, 20);
    Object_SetModeById_1_02002778(0, 3);
    Object_SetModeById_2_02002778(1, 3);
    Object_SetModeById_3_02002778(2, 3);
    Motion_CallWaitAnim_16(3, 3);
    Battle_WaitMode0_34(20);
    Battle_RunThenWaitIfModeZero_30(8, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_11_02002778(1, 0x103, 0);
    ObjectMotion_SetVariantCallback_7_02002778(1, 2);
    Battle_WaitMode0_35(60);
    Battle_RunThenWaitIfModeZero_31(1, 0, 20);
    Motion_CallWaitAnim_17(8, 4);
    Battle_WaitMode0_36(20);
    Battle_RunThenWaitIfModeZero_32(8, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_12_02002778(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_13_02002778(1, 0x101, 60);
    Battle_RunThenWaitIfModeZero_33(1, 0, 20);
    BattleFx_SpawnLinked_14(8, 0x102, 0);
    ObjectMotion_SetVariantCallback_8_02002778(8, 1);
    Battle_WaitMode0_37(60);
    Motion_CallWaitAnim_18(8, 4);
    Battle_WaitMode0_38(20);
    Battle_RunThenWaitIfModeZero_34(8, 0, 20);
    Battle_RunThenWaitIfModeZero_35(8, 0, 20);
    Motion_SetVarCb_9(0, 1);
    Motion_SetVarCb_10(1, 1);
    Motion_SetVarCb_11(2, 1);
    Motion_SetVarCbObj_8(3, 1);
    Battle_WaitMode0_39(20);
    Motion_CallWaitAnim_19(8, 4);
    Battle_WaitMode0_40(20);
    Battle_RunThenWaitIfModeZero_36(8, 0, 20);
    BattleFx_SpawnLinked_15(1, 0x101, 60);
    Motion_ArmCb_28(1, 0x5000, 20);
    BattleEventRuntime_ProcessAction_2_02002778(1, 0);
    Battle_WaitMode0_41(20);
    Motion_SetVarCbObj_9(2, 1);
    Battle_WaitMode0_42(20);
    Battle_RunThenWaitIfModeZero_37(2, 0, 20);
    Motion_ArmCb_29(1, 0xa000, 20);
    Motion_ArmCb_30(8, 0xc000, 20);
    Battle_RunThenWaitIfModeZero_38(8, 0, 20);
    Object_SetModeById_4_02002778(0, 3);
    Object_SetModeById_5_02002778(1, 3);
    Object_SetModeById_6_02002778(2, 3);
    Motion_CallWaitAnim_20(3, 3);
    Battle_WaitMode0_43(20);
    Motion_ArmCb_31(8, 0x4000, 20);
    Battle_RunThenWaitIfModeZero_39(8, 0, 20);
    Motion_SetVarCb_12(0, 2);
    Motion_SetVarCb_13(1, 2);
    Motion_SetVarCb_14(2, 2);
    Motion_SetVarCbObj_10(3, 2);
    Battle_WaitMode0_44(20);
    Motion_CallWaitAnim_21(8, 4);
    Battle_WaitMode0_45(20);
    Battle_RunThenWaitIfModeZero_40(8, 0, 20);
    BattleFx_SpawnLinked_16(2, 0x101, 60);
    Battle_RunThenWaitIfModeZero_41(2, 0, 20);
    Motion_CallWaitAnim_22(8, 3);
    Battle_WaitMode0_46(20);
    Battle_RunThenWaitIfModeZero_42(8, 0, 20);
    Battle_RunThenWaitIfModeZero_43(8, 0, 20);
    Battle_WaitMode0_47(0, 0x102);
    Battle_WaitMode0_48(1, 0x102);
    Battle_WaitMode0_49(2, 0x102);
    Battle_WaitMode0_50(3, 0x102);
    Battle_WaitMode0_51(60);
    Battle_RunThenWaitIfModeZero_44(3, 0, 20);
    Motion_ArmCb_32(8, 0x5000, 20);
    Battle_WaitMode0_52(20);
    Battle_RunThenWaitIfModeZero_45(8, 0, 20);
    BattleFx_SpawnLinked_17(0, 0x101, 0);
    BattleFx_SpawnLinked_18(1, 0x101, 0);
    BattleFx_SpawnLinked_19(2, 0x101, 0);
    BattleFx_SpawnLinked_20(3, 0x101, 60);
    Motion_SetVarCbObj_11(1, 1);
    Battle_WaitMode0_53(20);
    Battle_RunThenWaitIfModeZero_46(1, 0, 20);
    ObjectMotion_SetAngleToward_7_02002778(0, 1, 0);
    ObjectMotion_SetAngleToward_8_02002778(2, 1, 0);
    ObjectMotion_SetAngleToward_9_02002778(3, 1, 0);
    Battle_WaitMode0_54(20);
    Object_SetModeById_7_02002778(0, 3);
    Object_SetModeById_8_02002778(2, 3);
    Motion_CallWaitAnim_23(3, 3);
    Battle_WaitMode0_55(60);
    Motion_ArmCb_33(0, 0xc000, 0);
    Motion_ArmCb_34(2, 0xc000, 0);
    Motion_ArmCb_35(3, 0xd000, 0);
    Battle_WaitMode0_56(20);
    BattleEventRuntime_ProcessAction_3(8, 0);
    BattleFx_SpawnLinked_21(0, 0x102, 0);
    BattleFx_SpawnLinked_22(1, 0x102, 0);
    BattleFx_SpawnLinked_23(2, 0x102, 0);
    BattleFx_SpawnLinked_24(3, 0x102, 0);
    Battle_WaitMode0_57(60);
    Motion_ArmCb_36(1, 0x6000, 20);
    BattleEventRuntime_ProcessAction_4(1, 0);
    Battle_WaitMode0_58(20);
    Motion_ArmCb_37(3, 0x2000, 20);
    Battle_RunThenWaitIfModeZero_47(3, 0, 20);
    Motion_ArmCb_38(1, 0xa000, 0);
    Motion_ArmCb_39(3, 0xe000, 30);
    Motion_ArmCb_40(8, 0, 20);
    BattleFx_SpawnLinked_25(8, 0x105, 60);
    BattleFx_SpawnLinked_26(2, 0x101, 60);
    Battle_RunThenWaitIfModeZero_48(2, 0, 20);
    Motion_ArmCb_41(8, 0xc000, 20);
    BattleFx_SpawnLinked_27(8, 0x105, 60);
    Battle_RunThenWaitIfModeZero_49(8, 0, 20);
    BattleFx_SpawnLinked_28(0, 0x101, 0);
    BattleFx_SpawnLinked_29(1, 0x101, 0);
    BattleFx_SpawnLinked_30(2, 0x101, 0);
    BattleFx_SpawnLinked_31(3, 0x101, 60);
    Battle_RunThenWaitIfModeZero_50(8, 0, 20);
    BattleFx_SpawnLinked_32(0, 0x101, 0);
    BattleFx_SpawnLinked_33(1, 0x101, 0);
    BattleFx_SpawnLinked_34(2, 0x101, 0);
    BattleFx_SpawnLinked_35(3, 0x101, 60);
    Battle_RunThenWaitIfModeZero_51(8, 0, 20);
    Object_SetModeById_9_02002778(0, 3);
    Object_SetModeById_10_02002778(1, 3);
    Object_SetModeById_11_02002778(2, 3);
    Motion_CallWaitAnim_24(3, 3);
    Battle_WaitMode0_59(20);
    Motion_SetVarCbObj_12(8, 1);
    Battle_WaitMode0_60(20);
    Battle_RunThenWaitIfModeZero_52(8, 0, 20);
    Motion_ArmCb_42(0, 0, 0);
    Motion_ArmCb_43(1, 0x6000, 0);
    Motion_ArmCb_44(2, 0x8000, 0);
    Motion_ArmCb_45(3, 0x2000, 0);
    Battle_WaitMode0_61(60);
    Motion_ArmCb_46(0, 0xc000, 0);
    Motion_ArmCb_47(1, 0xa000, 0);
    Motion_ArmCb_48(2, 0xc000, 0);
    Motion_ArmCb_49(3, 0xe000, 0);
    Battle_WaitMode0_62(30);
    Motion_ArmCb_50(8, 0x4000, 20);
    Motion_CallWaitAnim_25(8, 4);
    Battle_WaitMode0_63(20);
    Battle_RunThenWaitIfModeZero_53(8, 0, 20);
    Motion_SetHPosTerrain_4(10, 0x1d80000, 0x2600000);
    Battle_RunThenWaitIfModeZero_54(10, 0, 20);
    BattleFx_SpawnLinked_36(0, 0x100, 0);
    BattleFx_SpawnLinked_37(1, 0x100, 0);
    BattleFx_SpawnLinked_38(2, 0x100, 0);
    BattleFx_SpawnLinked_39(3, 0x100, 0);
    BattleFx_SpawnLinked_40(8, 0x100, 0);
    BattleFx_SpawnLinked_41(9, 0x100, 0);
    Battle_WaitMode0_64(60);
    Motion_ArmCb_51(0, 0x4000, 0);
    Motion_ArmCb_52(1, 0x4000, 0);
    Motion_ArmCb_53(2, 0x4000, 0);
    Motion_ArmCb_54(3, 0x4000, 0);
    Motion_ArmCb_55(9, 0, 0);
    Battle_WaitMode0_65(30);
    Motion_SetSpeed_7(10, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1_02002778(10, 0x1d8, 0x218);
    BattleFx_SpawnLinked_42(8, 0x101, 60);
    Battle_RunThenWaitIfModeZero_55(8, 0, 20);
    Motion_ArmCb_56(2, 0xc000, 40);
    ObjectMotion_SetPositionAndReset_2_02002778(2, 0x1e8, 0x200);
    Battle_WaitMode0_66(10);
    Motion_ArmCb_57(2, 0x6000, 20);
    Motion_ArmCb_58(0, 0, 30);
    ObjectMotion_SetPositionAndReset_3_02002778(0, 0x1c8, 0x200);
    Battle_WaitMode0_67(10);
    Motion_ArmCb_59(0, 0x2000, 20);
    Motion_SetVarCbObj_13(10, 2);
    Battle_WaitMode0_68(20);
    Battle_RunThenWaitIfModeZero_56(10, 0, 20);
    BattleFx_SpawnLinked_43(8, 0x101, 60);
    Battle_RunThenWaitIfModeZero_57(8, 0, 20);
    Motion_SetVarCb_15(10, 2);
    Battle_WaitMode0_69(10, 0x102);
    Battle_WaitMode0_70(60);
    Battle_RunThenWaitIfModeZero_58(10, 0, 20);
    BattleFx_SpawnLinked_44(8, 0x101, 60);
    Battle_RunThenWaitIfModeZero_59(8, 0, 20);
    BattleFx_SpawnLinked_45(10, 0x100, 60);
    Battle_RunThenWaitIfModeZero_60(10, 0, 20);
    Motion_SetVarCb_16(8, 1);
    BattleFx_SpawnLinked_46(8, 0x100, 60);
    Battle_RunThenWaitIfModeZero_61(8, 0, 20);
    Motion_ArmCb_60(0, 0xe000, 0);
    Motion_ArmCb_61(1, 0xa000, 0);
    Motion_ArmCb_62(2, 0xa000, 0);
    Motion_ArmCb_63(3, 0xe000, 0);
    Battle_WaitMode0_71(40);
    Motion_CallWaitAnim_26(10, 3);
    Battle_WaitMode0_72(20);
    Battle_RunThenWaitIfModeZero_62(10, 0, 20);
    Motion_CallWaitAnim_27(8, 4);
    Battle_WaitMode0_73(20);
    Battle_RunThenWaitIfModeZero_63(8, 0, 20);
    BattleFx_SpawnLinked_47(8, 0x101, 60);
    Battle_RunThenWaitIfModeZero_64(8, 0, 20);
    Motion_SetVarCb_17(17, 1);
    Battle_WaitMode0_74(20);
    Battle_RunThenWaitIfModeZero_65(10, 0, 20);
    BattleFx_SpawnLinked_48(8, 0x105, 60);
    Battle_RunThenWaitIfModeZero_66(8, 0, 20);
    BattleFx_SpawnLinked_49(8, 0x100, 30);
    Battle_RunThenWaitIfModeZero_67(8, 0, 20);
    Object_SetModeById_12_02002778(0, 3);
    Battle_WaitMode0_75(2);
    Object_SetModeById_13_02002778(2, 3);
    Battle_WaitMode0_76(1);
    Object_SetModeById_14_02002778(3, 3);
    Battle_WaitMode0_77(5);
    Motion_CallWaitAnim_28(1, 3);
    ObjectMotion_SetAngleToward_10_02002778(8, 0, 0);
    Battle_WaitMode0_78(20);
    Battle_RunThenWaitIfModeZero_68(8, 0, 20);
    Motion_SetSpeed_8(10, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_4_02002778(10, 0x1d8, 0x1f8);
    Battle_WaitMode0_79(20);
    Motion_SetVarCb_18(10, 1);
    Battle_WaitMode0_80(20);
    Battle_RunThenWaitIfModeZero_69(10, 0, 20);
    ObjectMotion_SetAngleToward_11_02002778(8, 10, 0);
    Motion_SetSpeed_9(8, 0xcccc, 0x6666);
    Motion_ArmCb_64(0, 0x2000, 0);
    Motion_ArmCb_65(1, 0x6000, 0);
    Motion_ArmCb_66(2, 0x6000, 0);
    Motion_ArmCb_67(3, 0x2000, 0);
    Motion_CommitPos_6(8);
    Battle_WaitMode0_81(30);
    Motion_CallWaitAnim_29(8, 3);
    Battle_WaitMode0_82(20);
    Battle_RunThenWaitIfModeZero_70(8, 0, 20);
    Motion_ResetPosMode2_6(10, 0x1d8, 0x238);
    ObjectMotion_SetPositionAndReset_5_02002778(8, 0x1d8, 0x218);
    Motion_SetVarCbObj_14(1, 2);
    Motion_CommitPos_7(10);
    Battle_RunThenWaitIfModeZero_71(1, 0, 20);
    Motion_SetVarCb_19(8, 2);
    BattleFx_SpawnLinked_50(8, 0x102, 60);
    Call3_02002ad0((void (*)())Func_02007e46, 10, 0xd000, 0);
    Motion_ArmCb_68(8, 0xd000, 20);
    Battle_RunThenWaitIfModeZero_72(8, 0, 20);
    Motion_SetVarCb_20(0, 2);
    Motion_SetVarCb_21(1, 2);
    Motion_SetVarCb_22(2, 2);
    Motion_SetVarCbObj_15(3, 2);
    ObjectMotion_SetPositionAndReset_6_02002778(8, 0x1d8, 0x200);
    Motion_ArmCb_69(8, 0, 0);
    Motion_ArmCb_70(0, 0, 0);
    Motion_ArmCb_71(2, 0x8000, 0);
    Motion_ArmCb_72(9, 0xd000, 20);
    Motion_CallWaitAnim_30(8, 3);
    Battle_WaitMode0_83(20);
    Battle_RunThenWaitIfModeZero_73(8, 0, 20);
    Motion_CallWaitAnim_31(2, 3);
    Battle_WaitMode0_84(20);
    /* Clear bit 0 of the flag byte at +90. */
    Scene_GetRecord_4_02002778(0)[90] &= 0xfe;
    Motion_SetPosReset_7(0, 0x1c0, 0x200);
    Battle_WaitMode0_85(1);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 *record = Scene_GetRecord_5_02002778(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Battle_WaitMode0_86(20);
    Func_02007c42();
    Battle_WaitMode0_87(60);
    Func_02007e60(2, 144);
    Motion_CallWaitAnim_32(8, 3);
    Battle_WaitMode0_88(20);
    Battle_RunThenWaitIfModeZero_74(8, 0, 20);
    Object_SetModeById_15_02002778(0, 3);
    Object_SetModeById_16_02002778(1, 3);
    Object_SetModeById_17_02002778(2, 3);
    Motion_CallWaitAnim_33(3, 3);
    Battle_WaitMode0_89(20);
    Motion_ArmCb_73(0, 0x2000, 0);
    Motion_ArmCb_74(1, 0x6000, 0);
    Motion_ArmCb_75(2, 0x6000, 0);
    Motion_ArmCb_76(3, 0x2000, 0);
    Motion_ArmCb_77(9, 0x3000, 0);
    Battle_WaitMode0_90(20);
    Motion_SetPosReset_8(8, 0x1d8, 0x228);
    Motion_SetVarCbObj_16(8, 2);
    Battle_WaitMode0_91(20);
    Motion_ArmCb_78(8, 0xd000, 20);
    Battle_RunThenWaitIfModeZero_75(8, 0, 20);
    Motion_CallWaitAnim_34(8, 3);
    Battle_WaitMode0_92(20);
    Battle_RunThenWaitIfModeZero_76(8, 0, 20);
    Motion_SetSpeed_10(8, 0x8000, 0x4000);
    Motion_SetPosReset_9(8, 0x1e0, 0x21c);
    Battle_RunThenWaitIfModeZero_77(8, 0, 20);
    Motion_SetSpeed_11(8, 0xcccc, 0x6666);
    Motion_SetSpeed_12(9, 0xcccc, 0x6666);
    Motion_ResetPosMode2_7(8, 0x1d8, 0x260);
    Motion_ResetPosMode2_8(9, 0x1d8, 0x220);
    Motion_SetPosReset_10(10, 0x1d8, 0x260);
    Motion_CommitPos_8(9);
    Motion_ResetPosMode2_9(9, 0x1d8, 0x260);
    Motion_SetHPosTerrain_5(10, 0, 0);
    Motion_CommitPos_9(8);
    Motion_SetHPosTerrain_6(8, 0, 0);
    Motion_CommitPos_10(9);
    Motion_SetHPosTerrain_7(9, 0, 0);
    Motion_SetPosReset_11(2, 0x1e8, 0x208);
    BattleFx_SpawnLinked_51(2, 0x101, 60);
    Motion_SetVarCbObj_17(3, 1);
    Battle_WaitMode0_93(20);
    Motion_ArmCb_79(3, 0x2000, 20);
    Battle_RunThenWaitIfModeZero_78(3, 0, 20);
    Motion_ArmCb_80(0, 0, 0);
    Motion_ArmCb_81(2, 0xa000, 20);
    Motion_CallWaitAnim_35(2, 4);
    Battle_WaitMode0_94(20);
    Battle_RunThenWaitIfModeZero_79(2, 0, 20);
    BattleFx_SpawnLinked_52(3, 0x105, 60);
    Motion_SetVarCbObj_18(1, 1);
    Motion_ArmCb_82(1, 0x6000, 20);
    BattleEventRuntime_ProcessAction_5(1, 0);
    Motion_CallWaitAnim_36(0, 3);
    Battle_WaitMode0_95(20);
    Motion_SetVarCbObj_19(3, 2);
    Battle_WaitMode0_96(20);
    Motion_ArmCb_83(0, 0xc000, 0);
    Motion_ArmCb_84(1, 0x8000, 0);
    Battle_RunThenWaitIfModeZero_80(3, 0, 20);
    BattleFx_SpawnLinked_53(0, 0x101, 0);
    BattleFx_SpawnLinked_54(1, 0x101, 60);
    Motion_CallWaitAnim_37(3, 4);
    Battle_WaitMode0_97(20);
    Battle_RunThenWaitIfModeZero_81(3, 0, 20);
    BattleFx_SpawnLinked_55(0, 0x102, 0);
    BattleFx_SpawnLinked_56(1, 0x102, 60);
    Motion_SetSpeed_13(3, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Battle_WaitMode0_98(20);
        Motion_CallWaitAnim_38(1, 3);
        Battle_WaitMode0_99(20);
        Battle_RunThenWaitIfModeZero_82(1, 0, 20);
        /* Same step counter as bump_step(), incremented inline here. */
        *(u16 *)((*(u8 **)0x03001ebc + 0x1d8)) += 1;
    } else {
        Battle_WaitMode0_100(20);
        /* Same step counter as bump_step(), incremented inline here. */
        *(u16 *)((*(u8 **)0x03001ebc + 0x1d8)) += 1;
        Motion_CallWaitAnim_39(3, 3);
        Battle_WaitMode0_101(20);
        Battle_RunThenWaitIfModeZero_83(3, 0, 20);
    }
    Motion_SetVarCbObj_20(1, 1);
    Battle_RunThenWaitIfModeZero_84(1, 0, 20);
    Object_SetModeById_18_02002778(0, 3);
    Motion_CallWaitAnim_40(3, 3);
    Battle_WaitMode0_102(20);
    Motion_ResetPosMode2_10(1, 0x1c0, 0x200);
    Motion_SetPosReset_12(3, 0x1c0, 0x200);
    Motion_SetHPosTerrain_8(3, 0, 0);
    Motion_CommitPos_11(1);
    Motion_SetHPosTerrain_9(1, 0, 0);
    Battle_WaitMode0_103(20);
    ObjectMotion_SetAngleToward_12_02002778(0, 2, 20);
    Battle_WaitMode0_104(30);
    ObjectMotion_SetAngleToward_13_02002778(2, 0, 20);
    Battle_RunThenWaitIfModeZero_85(2, 0, 20);
    BattleFx_SpawnLinked_57(0, 0x102, 60);
    Motion_SetVarCb_23(2, 2);
    Battle_WaitMode0_105(2, 0x102);
    Battle_WaitMode0_106(60);
    Motion_CallWaitAnim_41(2, 4);
    Battle_WaitMode0_107(20);
    Battle_RunThenWaitIfModeZero_86(2, 0, 20);
    Motion_SetPosReset_13(2, 0x1c0, 0x200);
    Motion_SetHPosTerrain_10(2, 0, 0);
    Battle_SchedShoulder_1();
    GameFlag_Set_1_02002778(0x895);
}

void Scene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1)
{
    extern u8 *Data_03001ebc;

    u8 *globalCtx;

    Func_020084b0(160 << 1);
    Func_02008638(141, 1);
    globalCtx = *(u8 **)0x03001f30;
    Func_0200864c(arg0, arg1);
    globalCtx[0x23] = 0;
    Func_02008666();
    Func_0200864c_a(1);
    Func_0200845a(1);
}

void Scene_FinishSequence(void)
{
    extern u8 *Data_03001ebc;

    Func_02008584(0, 1);
    Func_02008672(2);
    Func_0200868e();
}

void Scene_SpawnEightShots(void)
{
    extern u8 *Data_03001ebc;

    struct Descriptor_02000484 descriptor;
    u8 *record;
    u32 i;

    record = Func_0200854c(8);
    descriptor.field0 = 1;
    descriptor.field24 = 0x0119;
    descriptor.field28 = 0x0200d1d8;
    descriptor.field16 = 224 << 10;
    descriptor.field20 = 192 << 9;
    for (i = 0; i <= 7; i++) {
        Func_02008540(10);
        if (i & 1) {
            Func_020086f6(0x82);
        }
        Func_020042d6(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16) + 0xffe80000, 0,
                      0x9999, 0, 0x00360001, (u8 *)&descriptor);
    }
    Func_02008578(60);
}

void Scene_SelectActorModeFromInputBit(s32 arg0)
{
    extern u8 *Data_03001ebc;

    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Func_0200866c(arg0, 10);
    } else {
        Func_02008674(arg0, 9);
    }
}

void Scene_RunParticleRain(void)
{
    extern u8 *Data_03001ebc;

    struct Descriptor_020041ec descriptor;
    u8 *record;
    u32 i;
    s32 x;
    s32 y;
    s32 scale;

    Func_02008778(0x83);
    *(u32 *)(Func_02008606(8) + 108) = 0x0200c1c5;
    Func_020085e0(40);
    Func_0200872a(128 << 9, 0);
    Func_0200872a_a(0x205c54, 1);
    Func_02008740(60);
    Func_020085fe(40);
    Func_020087ac(0x83);
    *(u32 *)(Func_0200863a(2) + 108) = 0x0200c1c5;
    Func_02008612(120);
    record = Func_02008648(8);
    descriptor.field0 = 1;
    descriptor.field4 = 2;
    descriptor.field24 = 0x011d;
    for (i = 0; i <= 63; i++) {
        if ((i & 3) == 0) {
            Func_020087e2(246);
        }
        x = *(s32 *)(record + 8)
            + ((((u32)(Func_020085c6() * 3) << 4) >> 16) << 16)
            + 0xfff40000;
        y = *(s32 *)(record + 12)
            + ((((u32)Func_020085de() << 5) >> 16) << 16)
            + 0xfff00000;
        scale = (((u32)((u32)Func_020085f2() << 2) >> 16) << 15) + (128 << 8);
        Func_020043fe(x, y, *(s32 *)(record + 16), 0,
                      scale, 0, 152 << 13, (u8 *)&descriptor);
        Func_0200860e(2);
    }
    Func_02008850(220);
    Func_020086ae(30);
    Func_020087f0(128 << 9, 1);
    Func_02008806(60);
    Func_020086c4(40);
    *(u32 *)(Func_020086fa(8) + 108) = 0;
    *(u32 *)(Func_02008704(2) + 108) = 0;
    Func_02008798(8, 0);
    Func_020087a0(2, 0);
}
