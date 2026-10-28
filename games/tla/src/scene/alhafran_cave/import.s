.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
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
	.global Func_020013f0
	.thumb_func
Func_020013f0:
	overlay_veneer Actor_SetMode
	.global Func_020013f8
	.thumb_func
Func_020013f8:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02001400
	.thumb_func
Func_02001400:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02001408
	.thumb_func
Func_02001408:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02001420
	.thumb_func
Func_02001420:
	overlay_veneer Func_08020360
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02001430
	.thumb_func
Func_02001430:
	overlay_veneer Event_StartFar
	.global Func_02001438
	.thumb_func
Func_02001438:
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
	.global Func_02001488
	.thumb_func
Func_02001488:
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
	.global Func_020014b8
	.thumb_func
Func_020014b8:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_020014c8
	.thumb_func
Func_020014c8:
	overlay_veneer Func_080c8198
	.global Func_020014d0
	.thumb_func
Func_020014d0:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_020014e8
	.thumb_func
Func_020014e8:
	overlay_veneer Func_080c8210
	.global Func_020014f0
	.thumb_func
Func_020014f0:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001508
	.thumb_func
Func_02001508:
	overlay_veneer Func_080c8240
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02001520
	.thumb_func
Func_02001520:
	overlay_veneer Func_080c8480
	.global Func_02001528
	.thumb_func
Func_02001528:
	overlay_veneer Event_ResolvePendingActionsFar
	.global BattleEffect_StartItemBreak
	.thumb_func
BattleEffect_StartItemBreak:
	overlay_veneer BattleEffect_StartItemBreakFar
	.global BattleEffect_SnapScaleToFull
	.thumb_func
BattleEffect_SnapScaleToFull:
	overlay_veneer BattleEffect_SnapScaleToFullFar
	.global UpdateRisingParticleBurst
	.thumb_func
UpdateRisingParticleBurst:
	overlay_veneer UpdateRisingParticleBurstFar
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
	.global Actor_SetActionCallback
	.thumb_func
Actor_SetActionCallback:
	overlay_veneer Actor_SetActionCallbackFar
	.global Func_02001568
	.thumb_func
Func_02001568:
	overlay_veneer Func_080c86a8
	.global Func_02001570
	.thumb_func
Func_02001570:
	overlay_veneer Func_080c86e8
	.global Func_02001578
	.thumb_func
Func_02001578:
	overlay_veneer Func_080c8700
	.global Func_02001580
	.thumb_func
Func_02001580:
	overlay_veneer Func_080c8708
	.global Func_02001588
	.thumb_func
Func_02001588:
	overlay_veneer Func_080c8710
	.global Func_02001590
	.thumb_func
Func_02001590:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02001598
	.thumb_func
Func_02001598:
	overlay_veneer Func_080c87a8
	.global Func_020015a0
	.thumb_func
Func_020015a0:
	overlay_veneer Func_080c87f8
	.ifdef TLA_EDITION_ES
	.global Field_EndActorSpriteEffect
	.thumb_func
Field_EndActorSpriteEffect:
	overlay_veneer Field_StartPaletteTransitionFar
	.else
	.ifdef TLA_EDITION_IT
	.global Field_EndActorSpriteEffect
	.thumb_func
Field_EndActorSpriteEffect:
	overlay_veneer Field_StartPaletteTransitionFar
	.else
	.global Field_StartPaletteTransition
	.thumb_func
Field_StartPaletteTransition:
	overlay_veneer Field_StartPaletteTransitionFar
	.endif
	.endif
	.global Func_020015b0
	.thumb_func
Func_020015b0:
	overlay_veneer BattleEffect_StartArcEffectFar
	.global Func_020015b8
	.thumb_func
Func_020015b8:
	overlay_veneer BattleEffect_EndArcEffectFar
	.global Func_020015c0
	.thumb_func
Func_020015c0:
	overlay_veneer BattleEffect_WaitForArcGroupsFar
	.global Func_020015c8
	.thumb_func
Func_020015c8:
	overlay_veneer BattleEffect_SetObjectUpdateModeFar
	.global Func_020015d0
	.thumb_func
Func_020015d0:
	overlay_veneer Func_080c8840
	.global Func_020015d8
	.thumb_func
Func_020015d8:
	overlay_veneer Func_080c8878
	.global Func_020015e0
	.thumb_func
Func_020015e0:
	overlay_veneer Func_080c88a0
	.global Func_020015e8
	.thumb_func
Func_020015e8:
	overlay_veneer Func_080c88a8
	.global Func_020015f0
	.thumb_func
Func_020015f0:
	overlay_veneer Func_080c88b0
	.global Actor_ApplyRandomChildValues
	.thumb_func
Actor_ApplyRandomChildValues:
	overlay_veneer Actor_ApplyRandomChildValuesFar
	.global Func_02001600
	.thumb_func
Func_02001600:
	overlay_veneer Sound_PlayFar
