@ Import veneers after the SORU_NICHIGETSU overlay's code: fixed 8-byte
@ veneers through which the overlay calls main-image code, each loading its
@ target into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — SORU_NICHIGETSU overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Engine_TaskRemoveCallback
	.thumb_func
Engine_TaskRemoveCallback:
	overlay_veneer Scheduler_RemoveCallbackFar
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer Random16Far
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Map_ClearLayerEntryFlag
	.thumb_func
Map_ClearLayerEntryFlag:
	overlay_veneer Map_ClearLayerEntryFlagFar
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Engine_WorkSetValuesIfNonNegative
	.thumb_func
Engine_WorkSetValuesIfNonNegative:
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
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
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
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
	.global Engine_ActorJump
	.thumb_func
Engine_ActorJump:
	overlay_veneer ObjectMotion_LaunchFar
	.global Engine_ActorRunRepeatedMotion
	.thumb_func
Engine_ActorRunRepeatedMotion:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global ObjectGroup_ConfigureChildValue
	.thumb_func
ObjectGroup_ConfigureChildValue:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
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
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Engine_ActorShowEmote
	.thumb_func
Engine_ActorShowEmote:
	overlay_veneer BattleFx_SpawnLinkedFar
	.global Engine_ActorSetAttachedEffect
	.thumb_func
Engine_ActorSetAttachedEffect:
	overlay_veneer BattleFx_ConfigureLinkedObjectFar
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
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer Event_SetValue170Far
	.global InitializeSceneRecordBuffer
	.thumb_func
InitializeSceneRecordBuffer:
	overlay_veneer FieldEffect_InitSparklesFar
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
	.global Engine_EventCloseScreen
	.thumb_func
Engine_EventCloseScreen:
	overlay_veneer Event_ClearStatus1c6Far
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global BattleFx_SetQueuedSoundAndPlay
	.thumb_func
BattleFx_SetQueuedSoundAndPlay:
	overlay_veneer BattleFx_SetQueuedSoundAndPlayFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
