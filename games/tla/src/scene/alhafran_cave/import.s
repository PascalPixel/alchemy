.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
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
	overlay_veneer Object_SetMode
	.global Func_020013f8
	.thumb_func
Func_020013f8:
	overlay_veneer Object_SetPosition
	.global Func_02001400
	.thumb_func
Func_02001400:
	.ifdef TLA_EDITION_DE
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	overlay_veneer Map_CopyCellAttributeRectFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02001408
	.thumb_func
Func_02001408:
	.ifdef TLA_EDITION_DE
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Map_GetTerrainHeightFar
	.else
	overlay_veneer Func_080201f0
	.endif
	.endif
	.endif
	.endif
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02001420
	.thumb_func
Func_02001420:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020330
	.else
	overlay_veneer Func_08020360
	.endif
	.endif
	.endif
	.endif
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001430
	.thumb_func
Func_02001430:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02001438
	.thumb_func
Func_02001438:
	overlay_veneer EventRuntime_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02001488
	.thumb_func
Func_02001488:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
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
	.global Func_020014b8
	.thumb_func
Func_020014b8:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_020014c8
	.thumb_func
Func_020014c8:
	overlay_veneer Func_080c8198
	.global Func_020014d0
	.thumb_func
Func_020014d0:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
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
	.ifdef TLA_EDITION_ES
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	overlay_veneer Event_StartScreenTransitionFar
	.endif
	.endif
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_DelayEffectFramesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_DelayEffectFramesFar
	.else
	overlay_veneer Event_WaitTransitionFramesFar
	.endif
	.endif
	.global Func_02001520
	.thumb_func
Func_02001520:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global Func_02001528
	.thumb_func
Func_02001528:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global BattleFx_StartItemBreak
	.thumb_func
BattleFx_StartItemBreak:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8518
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8518
	.else
	overlay_veneer BattleFx_StartItemBreakFar
	.endif
	.endif
	.global BattleFx_SnapScaleToFull
	.thumb_func
BattleFx_SnapScaleToFull:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_GetSpecialValueFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_GetSpecialValueFar
	.else
	overlay_veneer BattleFx_SnapScaleToFullFar
	.endif
	.endif
	.global UpdateRisingParticleBurst
	.thumb_func
UpdateRisingParticleBurst:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StartItemBreakFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StartItemBreakFar
	.else
	overlay_veneer UpdateRisingParticleBurstFar
	.endif
	.endif
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Object_SetActionCallback
	.thumb_func
Object_SetActionCallback:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8670
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8670
	.else
	overlay_veneer Object_SetActionCallbackFar
	.endif
	.endif
	.global Func_02001568
	.thumb_func
Func_02001568:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8698
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8698
	.else
	overlay_veneer Func_080c86a8
	.endif
	.endif
	.global Func_02001570
	.thumb_func
Func_02001570:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86d8
	.else
	overlay_veneer Func_080c86e8
	.endif
	.endif
	.global Func_02001578
	.thumb_func
Func_02001578:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86f0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86f0
	.else
	overlay_veneer Func_080c8700
	.endif
	.endif
	.global Func_02001580
	.thumb_func
Func_02001580:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86f8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86f8
	.else
	overlay_veneer Func_080c8708
	.endif
	.endif
	.global Func_02001588
	.thumb_func
Func_02001588:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_02001590
	.thumb_func
Func_02001590:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.endif
	.endif
	.global Func_02001598
	.thumb_func
Func_02001598:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8798
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8798
	.else
	overlay_veneer Func_080c87a8
	.endif
	.endif
	.global Func_020015a0
	.thumb_func
Func_020015a0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87e8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87e8
	.else
	overlay_veneer Func_080c87f8
	.endif
	.endif
	.ifdef TLA_EDITION_ES
	.global Field_EndActorSpriteEffect
	.thumb_func
Field_EndActorSpriteEffect:
	overlay_veneer Field_BeginPaletteTransitionFar
	.else
	.ifdef TLA_EDITION_IT
	.global Field_EndActorSpriteEffect
	.thumb_func
Field_EndActorSpriteEffect:
	overlay_veneer Field_BeginPaletteTransitionFar
	.else
	.global Field_BeginPaletteTransition
	.thumb_func
Field_BeginPaletteTransition:
	overlay_veneer Field_BeginPaletteTransitionFar
	.endif
	.endif
	.global Func_020015b0
	.thumb_func
Func_020015b0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87f8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87f8
	.else
	overlay_veneer BattleFx_StartArcEffectFar
	.endif
	.endif
	.global Func_020015b8
	.thumb_func
Func_020015b8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_EndActorSpriteEffectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_EndActorSpriteEffectFar
	.else
	overlay_veneer BattleFx_EndArcEffectFar
	.endif
	.endif
	.global Func_020015c0
	.thumb_func
Func_020015c0:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StartArcEffectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StartArcEffectFar
	.else
	overlay_veneer BattleFx_WaitForArcGroupsFar
	.endif
	.endif
	.global Func_020015c8
	.thumb_func
Func_020015c8:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_EndArcEffectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_EndArcEffectFar
	.else
	overlay_veneer BattleFx_SetObjectTickModeFar
	.endif
	.endif
	.global Func_020015d0
	.thumb_func
Func_020015d0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8830
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8830
	.else
	overlay_veneer Func_080c8840
	.endif
	.endif
	.global Func_020015d8
	.thumb_func
Func_020015d8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8868
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8868
	.else
	overlay_veneer Func_080c8878
	.endif
	.endif
	.global Func_020015e0
	.thumb_func
Func_020015e0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8890
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8890
	.else
	overlay_veneer Func_080c88a0
	.endif
	.endif
	.global Func_020015e8
	.thumb_func
Func_020015e8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8898
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8898
	.else
	overlay_veneer Func_080c88a8
	.endif
	.endif
	.global Func_020015f0
	.thumb_func
Func_020015f0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88a0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88a0
	.else
	overlay_veneer Func_080c88b0
	.endif
	.endif
	.global ObjectGroup_ApplyRandomChildValues
	.thumb_func
ObjectGroup_ApplyRandomChildValues:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88b0
	.else
	overlay_veneer ObjectGroup_ApplyRandomChildValuesFar
	.endif
	.endif
	.global Func_02001600
	.thumb_func
Func_02001600:
	overlay_veneer Audio_PlayCue
