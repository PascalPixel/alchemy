@ Import veneers after the SHINDEN_HEYA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — SHINDEN_HEYA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Math_RemainderUnsigned
	.thumb_func
Math_RemainderUnsigned:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer Random16Far
	.global CalculateFacingAngle
	.thumb_func
CalculateFacingAngle:
	overlay_veneer ArcTan2Far
	.global Engine_MathSin
	.thumb_func
Engine_MathSin:
	overlay_veneer Trig_SinFar
	.global Engine_MathCos
	.thumb_func
Engine_MathCos:
	overlay_veneer Trig_CosFar
	.global Runtime_AllocateHeapBlock
	.thumb_func
Runtime_AllocateHeapBlock:
	overlay_veneer Runtime_AllocateHeapBlockFar
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global ObjectDispatch_Initialize
	.thumb_func
ObjectDispatch_Initialize:
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Object_CreateFar
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global UiWindow_Create
	.thumb_func
UiWindow_Create:
	overlay_veneer UiWindow_CreateFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global UiText_DrawResource
	.thumb_func
UiText_DrawResource:
	overlay_veneer UiText_DrawResourceFar
	.global UiText_ShowCenteredMessage
	.thumb_func
UiText_ShowCenteredMessage:
	overlay_veneer UiText_ShowCenteredMessageFar
	.global UiTextResource_Initialize
	.thumb_func
UiTextResource_Initialize:
	overlay_veneer UiTextResource_InitializeFar
	.global UiTextResource_SetPosition
	.thumb_func
UiTextResource_SetPosition:
	overlay_veneer UiTextResource_SetPositionFar
	.global UiTextResource_Release
	.thumb_func
UiTextResource_Release:
	overlay_veneer UiTextResource_ReleaseFar
	.global Ui_PrepareTransferForItem
	.thumb_func
Ui_PrepareTransferForItem:
	overlay_veneer ItemIcon_LoadTilesFar
	.global SaveState_CountRecordsExcludingFlagged
	.thumb_func
SaveState_CountRecordsExcludingFlagged:
	overlay_veneer SaveState_CountRecordsExcludingFlaggedFar
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
	.ifndef TBS_EDITION_JA
	.global Event_CallWithLastActiveObjectId
	.thumb_func
Event_CallWithLastActiveObjectId:
	overlay_veneer Event_CallWithLastActiveObjectIdFar
	.endif
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	.global ResolveFacingObject
	.thumb_func
ResolveFacingObject:
	.global Object_GetById
	.thumb_func
Object_GetById:
	.global GetActorState
	.thumb_func
GetActorState:
	overlay_veneer Object_GetByIdFar
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global Engine_ActorStop
	.thumb_func
Engine_ActorStop:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Engine_ActorMoveToAndWait
	.thumb_func
Engine_ActorMoveToAndWait:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
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
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.global Engine_ActorJump
	.thumb_func
Engine_ActorJump:
	overlay_veneer ObjectMotion_LaunchFar
	.global Engine_ActorStartRepeatedMotion
	.thumb_func
Engine_ActorStartRepeatedMotion:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Engine_ActorRunRepeatedMotion
	.thumb_func
Engine_ActorRunRepeatedMotion:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Engine_ActorFaceActor
	.thumb_func
Engine_ActorFaceActor:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Engine_ActorFaceEachOther
	.thumb_func
Engine_ActorFaceEachOther:
	overlay_veneer Object_LinkPairFar
	.global Engine_ActorSetChildValue
	.thumb_func
Engine_ActorSetChildValue:
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
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Engine_EventShowTwoMessagesAndWait
	.thumb_func
Engine_EventShowTwoMessagesAndWait:
	overlay_veneer Battle_ShowPairedUnitWorkAndWaitFar
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
	.global Engine_CameraMoveToActor
	.thumb_func
Engine_CameraMoveToActor:
	overlay_veneer Object_PlaceCurrentWithinCameraBoundsFar
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer Event_SetValue170Far
	.global Engine_ColorBufferApplyTarget
	.thumb_func
Engine_ColorBufferApplyTarget:
	overlay_veneer BattleFx_ApplyColorToTargetBufferFar
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
	.global Engine_SanctumOpen
	.thumb_func
Engine_SanctumOpen:
	overlay_veneer Shop_ConfirmActFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
	.global AudioCommand_WaitForCompletion
	.thumb_func
AudioCommand_WaitForCompletion:
	overlay_veneer AudioCommand_WaitForCompletionFar
