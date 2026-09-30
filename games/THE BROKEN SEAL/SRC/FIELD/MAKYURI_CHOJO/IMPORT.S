@ Import veneers after the MAKYURI_CHOJO overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — MAKYURI_CHOJO overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global IwramSignedDivideEntry
	.thumb_func
IwramSignedDivideEntry:
	overlay_veneer IwramSignedDivide
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer Random16Far
	.global Engine_MathSin
	.thumb_func
Engine_MathSin:
	overlay_veneer Trig_SinFar
	.global Engine_MathCos
	.thumb_func
Engine_MathCos:
	overlay_veneer Trig_CosFar
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildrenFar
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	.global CreateOverlayObject
	.thumb_func
CreateOverlayObject:
	overlay_veneer Object_CreateFar
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Object_CheckMovementCollision
	.thumb_func
Object_CheckMovementCollision:
	overlay_veneer Func_080091d8
	.global SetOverlayObjectMode
	.thumb_func
SetOverlayObjectMode:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global RunOverlayObjectCommand1
	.thumb_func
RunOverlayObjectCommand1:
	overlay_veneer Animation_SetStateField5Bits2To3Far
	.global Engine_WorkSetValuesIfNonNegative
	.thumb_func
Engine_WorkSetValuesIfNonNegative:
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	.global Engine_MapRenderWaitForValues
	.thumb_func
Engine_MapRenderWaitForValues:
	overlay_veneer Map_WaitWorkValuesBelow256Far
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBitFar
	.thumb_func
GameFlag_ClearBitFar:
	overlay_veneer Func_080770d0
	.global Owner_RefreshRatiosOnFlagFar
	.thumb_func
Owner_RefreshRatiosOnFlagFar:
	overlay_veneer Func_08077268
	.global Engine_EventWait
	.thumb_func
Engine_EventWait:
	overlay_veneer Battle_WaitMode0Far
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer Battle_ResetFar
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer BattleFx_FinishActionFar
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer Object_GetByIdFar
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_SetPositionAndCommitFar
	.thumb_func
ObjectMotion_SetPositionAndCommitFar:
	overlay_veneer Func_0808a0c0
	.global Engine_ActorWalkTo
	.thumb_func
Engine_ActorWalkTo:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global Engine_ActorWalkToAndWait
	.thumb_func
Engine_ActorWalkToAndWait:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Engine_ActorWaitForMove
	.thumb_func
Engine_ActorWaitForMove:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer Object_SetModeByIdFar
	.global Engine_ActorSetAnimationAndWait
	.thumb_func
Engine_ActorSetAnimationAndWait:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Engine_ActorStartRepeatedMotion
	.thumb_func
Engine_ActorStartRepeatedMotion:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Engine_ActorRunRepeatedMotion
	.thumb_func
Engine_ActorRunRepeatedMotion:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Object_LinkPairFar
	.thumb_func
Object_LinkPairFar:
	overlay_veneer Func_0808a150
	.global Engine_ActorSetChildValue
	.thumb_func
Engine_ActorSetChildValue:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global ObjectGroup_SetChildValue
	.thumb_func
ObjectGroup_SetChildValue:
	.global SetOverlayObjectSlot
	.thumb_func
SetOverlayObjectSlot:
	overlay_veneer ObjectGroup_SetChildValueFar
	.global Event_SetValue1d8Far
	.thumb_func
Event_SetValue1d8Far:
	overlay_veneer Func_0808a170
	.global UiText_OpenMessageAtObjectFar
	.thumb_func
UiText_OpenMessageAtObjectFar:
	overlay_veneer Func_0808a178
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer BattleEv_RunWaitFar
	.global Engine_EventShowMessageAndWait
	.thumb_func
Engine_EventShowMessageAndWait:
	overlay_veneer BattlePres_RunActionThenWaitIfModeZeroFar
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Engine_ActorSetSpritePriority
	.thumb_func
Engine_ActorSetSpritePriority:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Engine_ActorShowEmote
	.thumb_func
Engine_ActorShowEmote:
	overlay_veneer BattleFx_SpawnLinkedFar
	.global Engine_ActorSetAttachedEffect
	.thumb_func
Engine_ActorSetAttachedEffect:
	overlay_veneer BattleFx_ConfigureLinkedObjectFar
	.global Engine_CameraSetSpeed
	.thumb_func
Engine_CameraSetSpeed:
	overlay_veneer ObjectTable_AllocateAndSetObjectSpeedFar
	.global Engine_CameraMoveTo
	.thumb_func
Engine_CameraMoveTo:
	overlay_veneer Motion_CamBoundsFar
	.global Engine_CameraWaitForMove
	.thumb_func
Engine_CameraWaitForMove:
	overlay_veneer BattleFx_CommitObjectPositionAndWaitFar
	.global Battle_GetWorkObject1e0Far
	.thumb_func
Battle_GetWorkObject1e0Far:
	overlay_veneer Func_0808a228
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer Event_SetValue170Far
	.global BattleFx_SetWeightedResult
	.thumb_func
BattleFx_SetWeightedResult:
	overlay_veneer BattleFx_SetWeightedResultFar
	.global Party_SetFields1ceAnd1d0
	.thumb_func
Party_SetFields1ceAnd1d0:
	overlay_veneer Party_SetFields1ceAnd1d0Far
	.global Engine_ColorBufferApplyTarget
	.thumb_func
Engine_ColorBufferApplyTarget:
	overlay_veneer BattleFx_ApplyColorToTargetBufferFar
	.global Engine_ColorBufferApplySource
	.thumb_func
Engine_ColorBufferApplySource:
	overlay_veneer BattleFx_ApplyColorToSourceBufferFar
	.global Engine_ColorBufferInterpolate
	.thumb_func
Engine_ColorBufferInterpolate:
	overlay_veneer BattleFx_StartBufferInterpolationFar
	.global Engine_EventOpenScreen
	.thumb_func
Engine_EventOpenScreen:
	overlay_veneer Event_SetStatus1c6Far
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Engine_PsynergyPlayEffect
	.thumb_func
Engine_PsynergyPlayEffect:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Engine_PsynergyBegin
	.thumb_func
Engine_PsynergyBegin:
	overlay_veneer BattleFx_LoadActionEffectResourcesFar
	.global Engine_PsynergySetTarget
	.thumb_func
Engine_PsynergySetTarget:
	overlay_veneer BattleFx_SetupObjectPairFar
	.global Engine_PsynergyLowerHands
	.thumb_func
Engine_PsynergyLowerHands:
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.global BattleEffect_CleanupSceneObjectsFar
	.thumb_func
BattleEffect_CleanupSceneObjectsFar:
	overlay_veneer Func_0808a440
	.global Engine_PsynergyRaiseHands
	.thumb_func
Engine_PsynergyRaiseHands:
	overlay_veneer EventObject_InitializeFar
	.global Audio_PlayCueFromEventWorkFar
	.thumb_func
Audio_PlayCueFromEventWorkFar:
	overlay_veneer Func_0808a4f0
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
