.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global Import_GameFlagSet
	.thumb_func
Import_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02001b2c
	.thumb_func
Func_02001b2c:
	overlay_veneer Actor_SetMode
	.global Func_02001b34
	.thumb_func
Func_02001b34:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02001b44
	.thumb_func
Func_02001b44:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02001b4c
	.thumb_func
Func_02001b4c:
	overlay_veneer Actor_CommitPosition
	.global Import_MapCopyCellPictureIndicesRect
	.thumb_func
Import_MapCopyCellPictureIndicesRect:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Import_MapCopyCellAttributeRect
	.thumb_func
Import_MapCopyCellAttributeRect:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02001b64
	.thumb_func
Func_02001b64:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Import_FieldMessageShow
	.thumb_func
Import_FieldMessageShow:
	overlay_veneer FieldMessage_ShowFar
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer Party_RemoveActiveOwnerFar
	.global Import_EventWait
	.thumb_func
Import_EventWait:
	overlay_veneer FieldEvent_WaitFar
	.global Import_EventStart
	.thumb_func
Import_EventStart:
	overlay_veneer Event_StartFar
	.global Import_EventEnd
	.thumb_func
Import_EventEnd:
	overlay_veneer Event_EndFar
	.global Import_ActorGet
	.thumb_func
Import_ActorGet:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_RefreshSelectorById
	.thumb_func
Actor_RefreshSelectorById:
	overlay_veneer Actor_RefreshSelectorByIdFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02001be4
	.thumb_func
Func_02001be4:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Import_ActorSetAnimation
	.thumb_func
Import_ActorSetAnimation:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02001c1c
	.thumb_func
Func_02001c1c:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02001c24
	.thumb_func
Func_02001c24:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02001c34
	.thumb_func
Func_02001c34:
	overlay_veneer Func_080c8210
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001c4c
	.thumb_func
Func_02001c4c:
	overlay_veneer Func_080c8240
	.global Func_02001c54
	.thumb_func
Func_02001c54:
	overlay_veneer Event_SetExitRequestFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02001c7c
	.thumb_func
Func_02001c7c:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Func_02001c84
	.thumb_func
Func_02001c84:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global Func_02001c8c
	.thumb_func
Func_02001c8c:
	overlay_veneer BattleEffect_StopMainActorFar
	.global Func_02001c94
	.thumb_func
Func_02001c94:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Func_02001c9c
	.thumb_func
Func_02001c9c:
	overlay_veneer EventObject_InitializeFar
	.global Import_EventResolvePendingActions
	.thumb_func
Import_EventResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02001cbc
	.thumb_func
Func_02001cbc:
	overlay_veneer Event_NoOpFar
	.global Import_PartyGetLeaderActor
	.thumb_func
Import_PartyGetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02001ccc
	.thumb_func
Func_02001ccc:
	overlay_veneer Func_080c8798
	.global Func_02001cd4
	.thumb_func
Func_02001cd4:
	overlay_veneer Func_080c87a0
	.global Import_SoundPlay
	.thumb_func
Import_SoundPlay:
	overlay_veneer Sound_PlayFar
