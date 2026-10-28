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
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02003100
	.thumb_func
Func_02003100:
	overlay_veneer Actor_SetMode
	.global Func_02003108
	.thumb_func
Func_02003108:
	overlay_veneer Actor_StartScriptFar
	.global Func_02003110
	.thumb_func
Func_02003110:
	overlay_veneer Func_080200c0
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02003130
	.thumb_func
Func_02003130:
	overlay_veneer Event_StartFar
	.global Func_02003138
	.thumb_func
Func_02003138:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Func_02003148
	.thumb_func
Func_02003148:
	overlay_veneer Func_080c8078
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
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
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
	.global Func_02003198
	.thumb_func
Func_02003198:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
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
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_020031c8
	.thumb_func
Func_020031c8:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_020031d8
	.thumb_func
Func_020031d8:
	overlay_veneer Func_080c8198
	.global Func_020031e0
	.thumb_func
Func_020031e0:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_020031f0
	.thumb_func
Func_020031f0:
	overlay_veneer Battle_ShowPairedUnitWorkAndWaitFar
	.global Func_020031f8
	.thumb_func
Func_020031f8:
	overlay_veneer Func_080c8210
	.global Func_02003200
	.thumb_func
Func_02003200:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003218
	.thumb_func
Func_02003218:
	overlay_veneer Func_080c8240
	.global Func_02003220
	.thumb_func
Func_02003220:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02003240
	.thumb_func
Func_02003240:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02003248
	.thumb_func
Func_02003248:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Actor_OffsetPositionAndReset
	.thumb_func
Actor_OffsetPositionAndReset:
	overlay_veneer Actor_OffsetPositionAndResetFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Func_02003260
	.thumb_func
Func_02003260:
	overlay_veneer Func_080c87a8
	.global Func_02003268
	.thumb_func
Func_02003268:
	overlay_veneer Func_080c87f8
	.global Field_StartPaletteTransition
	.thumb_func
Field_StartPaletteTransition:
	overlay_veneer Field_StartPaletteTransitionFar
	.global Func_02003278
	.thumb_func
Func_02003278:
	overlay_veneer BattleEffect_StartArcEffectFar
	.global Func_02003280
	.thumb_func
Func_02003280:
	overlay_veneer BattleEffect_EndArcEffectFar
	.global Func_02003288
	.thumb_func
Func_02003288:
	overlay_veneer BattleEffect_WaitForArcGroupsFar
	.global Func_02003290
	.thumb_func
Func_02003290:
	overlay_veneer BattleEffect_SetObjectUpdateModeFar
	.global Func_02003298
	.thumb_func
Func_02003298:
	overlay_veneer Func_080c8860
	.global Func_020032a0
	.thumb_func
Func_020032a0:
	overlay_veneer Func_080c8930
	.global Func_020032a8
	.thumb_func
Func_020032a8:
	overlay_veneer Sound_PlayFar
