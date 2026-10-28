.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02001d28
	.thumb_func
Func_02001d28:
	overlay_veneer Func_080200c0
	.global Func_02001d30
	.thumb_func
Func_02001d30:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02001d40
	.thumb_func
Func_02001d40:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02001d48
	.thumb_func
Func_02001d48:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02001d50
	.thumb_func
Func_02001d50:
	overlay_veneer Actor_CommitPosition
	.global Func_02001d58
	.thumb_func
Func_02001d58:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02001d60
	.thumb_func
Func_02001d60:
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.global Func_02001d68
	.thumb_func
Func_02001d68:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02001d78
	.thumb_func
Func_02001d78:
	overlay_veneer Func_08020240
	.global Func_02001d80
	.thumb_func
Func_02001d80:
	overlay_veneer Func_08020248
	.global Func_02001d88
	.thumb_func
Func_02001d88:
	overlay_veneer Func_08038210
	.global Func_02001d90
	.thumb_func
Func_02001d90:
	overlay_veneer Func_08038248
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02001da0
	.thumb_func
Func_02001da0:
	overlay_veneer Event_StartFar
	.global Func_02001da8
	.thumb_func
Func_02001da8:
	overlay_veneer Event_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
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
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_SetMovementTypeAndWaitById
	.thumb_func
Actor_SetMovementTypeAndWaitById:
	overlay_veneer Actor_SetMovementTypeAndWaitByIdFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02001e10
	.thumb_func
Func_02001e10:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02001e18
	.thumb_func
Func_02001e18:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02001e20
	.thumb_func
Func_02001e20:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02001e48
	.thumb_func
Func_02001e48:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02001e58
	.thumb_func
Func_02001e58:
	overlay_veneer Func_080c8198
	.global Func_02001e60
	.thumb_func
Func_02001e60:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02001e70
	.thumb_func
Func_02001e70:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02001e80
	.thumb_func
Func_02001e80:
	overlay_veneer Func_080c8208
	.global Func_02001e88
	.thumb_func
Func_02001e88:
	overlay_veneer Func_080c8210
	.global Func_02001e90
	.thumb_func
Func_02001e90:
	overlay_veneer Func_080c8218
	.global Func_02001e98
	.thumb_func
Func_02001e98:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001ea8
	.thumb_func
Func_02001ea8:
	overlay_veneer Func_080c8240
	.global Func_02001eb0
	.thumb_func
Func_02001eb0:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02001eb8
	.thumb_func
Func_02001eb8:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02001ec0
	.thumb_func
Func_02001ec0:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02001ec8
	.thumb_func
Func_02001ec8:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
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
	.global Func_02001ee8
	.thumb_func
Func_02001ee8:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02001ef0
	.thumb_func
Func_02001ef0:
	overlay_veneer Party_PlaySavedMusicFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02001f00
	.thumb_func
Func_02001f00:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02001f08
	.thumb_func
Func_02001f08:
	overlay_veneer Func_080c8930
	.global Func_02001f10
	.thumb_func
Func_02001f10:
	overlay_veneer Sound_PlayFar
