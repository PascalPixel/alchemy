.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02001d20
	.thumb_func
Func_02001d20:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02001d28
	.thumb_func
Func_02001d28:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02001d38
	.thumb_func
Func_02001d38:
	overlay_veneer Func_08020300
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001d50
	.thumb_func
Func_02001d50:
	overlay_veneer Event_StartFar
	.global Func_02001d58
	.thumb_func
Func_02001d58:
	overlay_veneer Event_EndFar
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
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02001db8
	.thumb_func
Func_02001db8:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02001dc0
	.thumb_func
Func_02001dc0:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_WaitForAnimationChange
	.thumb_func
Actor_WaitForAnimationChange:
	overlay_veneer Actor_WaitForAnimationChangeFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Func_02001e08
	.thumb_func
Func_02001e08:
	overlay_veneer Actor_ConfigureSpriteColorFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02001e18
	.thumb_func
Func_02001e18:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02001e28
	.thumb_func
Func_02001e28:
	overlay_veneer Func_080c8198
	.global Func_02001e30
	.thumb_func
Func_02001e30:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02001e40
	.thumb_func
Func_02001e40:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Func_02001e48
	.thumb_func
Func_02001e48:
	overlay_veneer Func_080c8210
	.global Func_02001e50
	.thumb_func
Func_02001e50:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001e68
	.thumb_func
Func_02001e68:
	overlay_veneer Func_080c8240
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02001e80
	.thumb_func
Func_02001e80:
	overlay_veneer Func_080c8480
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02001e90
	.thumb_func
Func_02001e90:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Func_02001e98
	.thumb_func
Func_02001e98:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global Func_02001ea0
	.thumb_func
Func_02001ea0:
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.global Func_02001ea8
	.thumb_func
Func_02001ea8:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Func_02001eb0
	.thumb_func
Func_02001eb0:
	overlay_veneer EventObject_InitializeFar
	.global Func_02001eb8
	.thumb_func
Func_02001eb8:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02001ec0
	.thumb_func
Func_02001ec0:
	overlay_veneer Func_080c8580
	.global Func_02001ec8
	.thumb_func
Func_02001ec8:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Actor_OffsetPositionAndReset
	.thumb_func
Actor_OffsetPositionAndReset:
	overlay_veneer Actor_OffsetPositionAndResetFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02001ee8
	.thumb_func
Func_02001ee8:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02001ef0
	.thumb_func
Func_02001ef0:
	overlay_veneer Func_080c87a8
	.global Func_02001ef8
	.thumb_func
Func_02001ef8:
	overlay_veneer Func_080c8930
	.global Func_02001f00
	.thumb_func
Func_02001f00:
	overlay_veneer Func_08108010
	.global Func_02001f08
	.thumb_func
Func_02001f08:
	overlay_veneer Func_08108018
	.global Func_02001f10
	.thumb_func
Func_02001f10:
	overlay_veneer Sound_PlayFar
