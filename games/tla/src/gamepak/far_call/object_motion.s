.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Actor_GetByIdFar
	.thumb_func
Actor_GetByIdFar:
	overlay_veneer Actor_GetById
	.global ObjectTable_DestroyByIdFar
	.thumb_func
ObjectTable_DestroyByIdFar:
	overlay_veneer ObjectTable_DestroyById
	.global Actor_SetSpeedParametersFar
	.thumb_func
Actor_SetSpeedParametersFar:
	overlay_veneer Actor_SetSpeedParameters
	.global Actor_SetMovementTypeByIdFar
	.thumb_func
Actor_SetMovementTypeByIdFar:
	overlay_veneer Actor_SetMovementTypeById
	.global Actor_RefreshSelectorByIdFar
	.thumb_func
Actor_RefreshSelectorByIdFar:
	overlay_veneer Actor_RefreshSelectorById
	.global Actor_StopFar
	.thumb_func
Actor_StopFar:
	overlay_veneer Actor_Stop
	.global Actor_SetMovementTypeAndWaitByIdFar
	.thumb_func
Actor_SetMovementTypeAndWaitByIdFar:
	overlay_veneer Actor_SetMovementTypeAndWaitById
	.global Actor_ResetAndSetPositionFar
	.thumb_func
Actor_ResetAndSetPositionFar:
	overlay_veneer Actor_ResetAndSetPosition
	.global Actor_SetPositionAndCommitFar
	.thumb_func
Actor_SetPositionAndCommitFar:
	overlay_veneer Actor_SetPositionAndCommit
	.global Actor_ResetAndSetPositionInMode2Far
	.thumb_func
Actor_ResetAndSetPositionInMode2Far:
	overlay_veneer Actor_ResetAndSetPositionInMode2
	.global Actor_SetPositionAndResetFar
	.thumb_func
Actor_SetPositionAndResetFar:
	overlay_veneer Actor_SetPositionAndReset
	.global Actor_CenterOnColumnFar
	.thumb_func
Actor_CenterOnColumnFar:
	overlay_veneer Actor_CenterOnColumn
	.global Actor_OffsetPositionAndResetMotionFar
	.thumb_func
Actor_OffsetPositionAndResetMotionFar:
	overlay_veneer Actor_OffsetPositionAndResetMotion
	.global Actor_WaitForMoveByIdFar
	.thumb_func
Actor_WaitForMoveByIdFar:
	overlay_veneer Actor_WaitForMoveById
	.global Actor_SetHorizontalPositionWithTerrainFar
	.thumb_func
Actor_SetHorizontalPositionWithTerrainFar:
	overlay_veneer Actor_SetHorizontalPositionWithTerrain
	.global Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.thumb_func
Actor_SetHorizontalPositionAndAngleWithTerrainFar:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrain
	.global Actor_SetPositionWithTerrainFar
	.thumb_func
Actor_SetPositionWithTerrainFar:
	overlay_veneer Actor_SetPositionWithTerrain
	.global Actor_MoveToObjectFar
	.thumb_func
Actor_MoveToObjectFar:
	overlay_veneer Actor_MoveToObject
	.global Actor_SetModeByIdFar
	.thumb_func
Actor_SetModeByIdFar:
	overlay_veneer Actor_SetModeById
	.global Actor_SetActionByIdFar
	.thumb_func
Actor_SetActionByIdFar:
	overlay_veneer Actor_SetActionById
	.global Motion_SetModeAndWaitAnimationFar
	.thumb_func
Motion_SetModeAndWaitAnimationFar:
	overlay_veneer Motion_SetModeAndWaitAnimation
	.global Actor_WaitForAnimationChangeFar
	.thumb_func
Actor_WaitForAnimationChangeFar:
	overlay_veneer Actor_WaitForAnimationChange
	.global Actor_JumpFar
	.thumb_func
Actor_JumpFar:
	overlay_veneer Actor_Jump
	.global Actor_StartRepeatedMotionFar
	.thumb_func
Actor_StartRepeatedMotionFar:
	overlay_veneer Actor_StartRepeatedMotion
	.global Motion_SetVarCbAndRefreshFar
	.thumb_func
Motion_SetVarCbAndRefreshFar:
	overlay_veneer Motion_SetVarCbAndRefresh
	.global Actor_JumpDownLedgeFar
	.thumb_func
Actor_JumpDownLedgeFar:
	overlay_veneer Actor_JumpDownLedge
	.global Actor_SetAngleTowardFar
	.thumb_func
Actor_SetAngleTowardFar:
	overlay_veneer Actor_SetAngleToward
	.global Actor_TurnToFaceEachOtherFar
	.thumb_func
Actor_TurnToFaceEachOtherFar:
	overlay_veneer Actor_TurnToFaceEachOther
	.global Actor_ConfigureSpriteColorFar
	.thumb_func
Actor_ConfigureSpriteColorFar:
	overlay_veneer Actor_ConfigureSpriteColor
	.global Actor_SetPartAttributeFar
	.thumb_func
Actor_SetPartAttributeFar:
	overlay_veneer Actor_SetPartAttribute
	.global Func_080c8178
	.thumb_func
Func_080c8178:
	overlay_veneer Actor_SetTargetAndCallback
	.global Event_SetMessageIdFar
	.thumb_func
Event_SetMessageIdFar:
	overlay_veneer Event_SetMessageId
	.global FieldMessage_ShowAtActorFar
	.thumb_func
FieldMessage_ShowAtActorFar:
	overlay_veneer FieldMessage_ShowAtActor
	.global Func_080c8190
	.thumb_func
Func_080c8190:
	overlay_veneer Func_080d3fb0
	.global Func_080c8198
	.thumb_func
Func_080c8198:
	overlay_veneer Event_ShowMessageThenWait
	.global Event_ShowMessageAndWaitFar
	.thumb_func
Event_ShowMessageAndWaitFar:
	overlay_veneer Event_ShowMessageAndWait
	.global Func_080c81a8
	.thumb_func
Func_080c81a8:
	overlay_veneer Event_AskYesNoAndShowResponse
	.global Func_080c81b0
	.thumb_func
Func_080c81b0:
	overlay_veneer Event_NoOpUpdate
	.global Func_080c81b8
	.thumb_func
Func_080c81b8:
	overlay_veneer Func_080d40dc
	.global Func_080c81c0
	.thumb_func
Func_080c81c0:
	overlay_veneer Event_ShowCounter
	.global Event_ShowCounterAtPositionFar
	.thumb_func
Event_ShowCounterAtPositionFar:
	overlay_veneer Event_ShowCounterAtPosition
	.global Actor_ArmCallbackFar
	.thumb_func
Actor_ArmCallbackFar:
	overlay_veneer Actor_ArmCallback
	.global Actor_ArmCallbackAndRefreshFar
	.thumb_func
Actor_ArmCallbackAndRefreshFar:
	overlay_veneer Actor_ArmCallbackAndRefresh
	.global Actor_ResetTargetAndSetMode1Far
	.thumb_func
Actor_ResetTargetAndSetMode1Far:
	overlay_veneer Actor_ResetTargetAndSetMode1
	.global ObjectTable_RunIfActiveFar
	.thumb_func
ObjectTable_RunIfActiveFar:
	overlay_veneer ObjectTable_RunIfActive
	.global Battle_ShowPairedUnitWorkAndWaitFar
	.thumb_func
Battle_ShowPairedUnitWorkAndWaitFar:
	overlay_veneer Battle_ShowPairedUnitWorkAndWait
	.global Message_SetVoiceFromActorFar
	.thumb_func
Message_SetVoiceFromActorFar:
	overlay_veneer Func_080d4330
	.global Actor_SetSpritePriorityFar
	.thumb_func
Actor_SetSpritePriorityFar:
	overlay_veneer Actor_SetSpritePriority
	.global Func_080c8208
	.thumb_func
Func_080c8208:
	overlay_veneer Func_080d390c
	.global Func_080c8210
	.thumb_func
Func_080c8210:
	overlay_veneer Func_080d47b4
	.global Func_080c8218
	.thumb_func
Func_080c8218:
	overlay_veneer Func_080d489c
	.global ObjectVisual_CopyAttributesFar
	.thumb_func
ObjectVisual_CopyAttributesFar:
	overlay_veneer ObjectVisual_CopyAttributes
	.global Camera_FollowActorFar
	.thumb_func
Camera_FollowActorFar:
	overlay_veneer Camera_FollowActor
	.global Camera_SetSpeedParametersFar
	.thumb_func
Camera_SetSpeedParametersFar:
	overlay_veneer Camera_SetSpeedParameters
	.global Camera_MoveWithinMapBoundsFar
	.thumb_func
Camera_MoveWithinMapBoundsFar:
	overlay_veneer Camera_MoveWithinMapBounds
	.global Func_080c8240
	.thumb_func
Func_080c8240:
	overlay_veneer Func_080d4548
	.global Func_080c8248
	.thumb_func
Func_080c8248:
	overlay_veneer Func_080d450c
	.global Func_080c8250
	.thumb_func
Func_080c8250:
	overlay_veneer Func_080d452c
	.global Func_080c8258
	.thumb_func
Func_080c8258:
	overlay_veneer Func_080d4568
	.global Func_080c8260
	.thumb_func
Func_080c8260:
	overlay_veneer Func_080d45b8
