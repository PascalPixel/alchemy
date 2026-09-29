@ Import veneers after the FUNE_KANPAN overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — FUNE_KANPAN overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global IwramUnsignedRemainder
	.thumb_func
IwramUnsignedRemainder:
	.global BuildMotionCountdown
	.thumb_func
BuildMotionCountdown:
	overlay_veneer IwramUnsignedRemainderArm
	.global WaitFrames
	.thumb_func
WaitFrames:
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
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	.global Engine_ObjectSetPosition
	.thumb_func
Engine_ObjectSetPosition:
	overlay_veneer Object_SetMoveTargetFar
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global ObjectDispatch_SetSingleChildField26Far
	.thumb_func
ObjectDispatch_SetSingleChildField26Far:
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer Func_080091e0
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	.global Graphics_EnableObjLayerAndCallbacks
	.thumb_func
Graphics_EnableObjLayerAndCallbacks:
	overlay_veneer Graphics_EnableObjLayerAndCallbacksFar
	.global ObjectDispatch_StopCallbacksAndHideLayers
	.thumb_func
ObjectDispatch_StopCallbacksAndHideLayers:
	overlay_veneer ObjectDispatch_StopCallbacksAndHideLayersFar
	.global UiText_ShowCenteredMessage
	.thumb_func
UiText_ShowCenteredMessage:
	overlay_veneer UiText_ShowCenteredMessageFar
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	.global Engine_GameFlagClear
	.thumb_func
Engine_GameFlagClear:
	overlay_veneer GameFlag_ClearBitFar
	.global Engine_EventWait
	.thumb_func
Engine_EventWait:
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	.global Battle_Reset
	.thumb_func
Battle_Reset:
	overlay_veneer Battle_ResetFar
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer BattleFx_FinishActionFar
	.global Event_CallWithLastActiveObjectId
	.thumb_func
Event_CallWithLastActiveObjectId:
	overlay_veneer Event_CallWithLastActiveObjectIdFar
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Engine_ActorDestroy
	.thumb_func
Engine_ActorDestroy:
	overlay_veneer ObjectTable_DestroyByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	.global Engine_ActorStartAction
	.thumb_func
Engine_ActorStartAction:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global Engine_ActorStop
	.thumb_func
Engine_ActorStop:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer Object_SetActionCallbackAndRefreshByIdFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global Engine_ObjectMotionSetPositionAndCommit
	.thumb_func
Engine_ObjectMotionSetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	.global Engine_ActorWalkTo
	.thumb_func
Engine_ActorWalkTo:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	.global Engine_ActorWalkToAndWait
	.thumb_func
Engine_ActorWalkToAndWait:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_SnapHeadingAndOffset
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	overlay_veneer ObjectMotion_SnapHeadingAndOffsetFar
	.global ObjectMotion_SetHorizontalPositionWithTerrain
	.thumb_func
ObjectMotion_SetHorizontalPositionWithTerrain:
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
	.global Engine_ActorJump
	.thumb_func
Engine_ActorJump:
	overlay_veneer ObjectMotion_LaunchFar
	.global Engine_ActorStartRepeatedMotion
	.thumb_func
Engine_ActorStartRepeatedMotion:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	.global Engine_ActorRunRepeatedMotion
	.thumb_func
Engine_ActorRunRepeatedMotion:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Engine_ActorFaceActor
	.thumb_func
Engine_ActorFaceActor:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global ObjectGroup_ConfigureChildValue
	.thumb_func
ObjectGroup_ConfigureChildValue:
	.global Engine_ActorSetChildValue
	.thumb_func
Engine_ActorSetChildValue:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global Event_SetValue1d8
	.thumb_func
Event_SetValue1d8:
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer Event_SetValue1d8Far
	.global Engine_EventOpenMessage
	.thumb_func
Engine_EventOpenMessage:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer BattleEv_RunWaitFar
	.global Engine_EventShowMessageAndWait
	.thumb_func
Engine_EventShowMessageAndWait:
	overlay_veneer BattlePres_RunActionThenWaitIfModeZeroFar
	overlay_veneer BattleEventRuntime_ProcessActionFar
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Ui_SetRenderResultFromObject
	.thumb_func
Ui_SetRenderResultFromObject:
	overlay_veneer Ui_SetRenderResultFromObjectFar
	.global Engine_ActorSetSpritePriority
	.thumb_func
Engine_ActorSetSpritePriority:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Engine_ActorShowEmote
	.thumb_func
Engine_ActorShowEmote:
	overlay_veneer BattleFx_SpawnLinkedFar
	overlay_veneer BattleFx_ConfigureLinkedObjectFar
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global Engine_CameraFollowActor
	.thumb_func
Engine_CameraFollowActor:
	overlay_veneer Object_AttachWorkTargetToObjectFar
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
	.global Event_SetValue170
	.thumb_func
Event_SetValue170:
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer Event_SetValue170Far
	overlay_veneer BattleFx_SetWeightedResultFar
	overlay_veneer Party_SetFields1ceAnd1d0Far
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	.global Engine_EventOpenScreen
	.thumb_func
Engine_EventOpenScreen:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_ClearStatus1c6Far
	.thumb_func
Event_ClearStatus1c6Far:
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	.global Engine_EventCloseScreen
	.thumb_func
Engine_EventCloseScreen:
	overlay_veneer Func_0808a368
	.global Event_WaitValue1c8FramesFar
	.thumb_func
Event_WaitValue1c8FramesFar:
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer Func_0808a370
	.global Battle_ResetEffectCounter
	.thumb_func
Battle_ResetEffectCounter:
	overlay_veneer Battle_ResetEffectCounterFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	.global AudioCommand_Play
	.thumb_func
AudioCommand_Play:
	overlay_veneer AudioCommand_PlayFar
