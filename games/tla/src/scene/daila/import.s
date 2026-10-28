.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global Import_GameFlagIsSet
	.thumb_func
Import_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global Import_GameFlagSet
	.thumb_func
Import_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02000c48
	.thumb_func
Func_02000c48:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Import_EventWaitFrames
	.thumb_func
Import_EventWaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Import_EventStart
	.thumb_func
Import_EventStart:
	overlay_veneer Event_StartFar
	.global Import_EventEnd
	.thumb_func
Import_EventEnd:
	overlay_veneer Event_EndFar
	.global Import_EventAskYesNoWithActorAnimation
	.thumb_func
Import_EventAskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Import_ActorGet
	.thumb_func
Import_ActorGet:
	overlay_veneer Object_GetByIdFar
	.global Import_ActorSetSpeed
	.thumb_func
Import_ActorSetSpeed:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Import_ActorCenterAndWalk
	.thumb_func
Import_ActorCenterAndWalk:
	overlay_veneer ObjectMotion_CenterOnColumnFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02000ca8
	.thumb_func
Func_02000ca8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Import_ActorSetAnimation
	.thumb_func
Import_ActorSetAnimation:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_TurnToFaceEachOther
	.thumb_func
Object_TurnToFaceEachOther:
	overlay_veneer Object_LinkPairFar
	.global Import_EventSetMessageId
	.thumb_func
Import_EventSetMessageId:
	overlay_veneer Event_SetMessageIdFar
	.global Import_FieldMessageShowAtActor
	.thumb_func
Import_FieldMessageShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Import_EventShowMessageAndWait
	.thumb_func
Import_EventShowMessageAndWait:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Import_ActorSetSpritePriority
	.thumb_func
Import_ActorSetSpritePriority:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02000d08
	.thumb_func
Func_02000d08:
	overlay_veneer Func_080c8210
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02000d18
	.thumb_func
Func_02000d18:
	overlay_veneer Func_080c82e0
	.global Import_EventFinishScreenTransition
	.thumb_func
Import_EventFinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Import_EventWaitTransitionFrames
	.thumb_func
Import_EventWaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Object_SetWideSprite
	.thumb_func
Object_SetWideSprite:
	overlay_veneer Object_SetWideSpriteFar
	.global Import_ActorReturnHome
	.thumb_func
Import_ActorReturnHome:
	overlay_veneer Object_ReturnHomeFar
	.global Import_EventResolvePendingActions
	.thumb_func
Import_EventResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02000d48
	.thumb_func
Func_02000d48:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Func_02000d50
	.thumb_func
Func_02000d50:
	overlay_veneer Func_080c86f8
	.global Func_02000d58
	.thumb_func
Func_02000d58:
	overlay_veneer Func_080c8708
	.global Import_PartyGetLeaderActor
	.thumb_func
Import_PartyGetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02000d68
	.thumb_func
Func_02000d68:
	overlay_veneer Func_080c87b0
	.global Import_ShopOpen
	.thumb_func
Import_ShopOpen:
	overlay_veneer Func_08108008
	.global Import_SoundPlay
	.thumb_func
Import_SoundPlay:
	overlay_veneer Sound_PlayFar
