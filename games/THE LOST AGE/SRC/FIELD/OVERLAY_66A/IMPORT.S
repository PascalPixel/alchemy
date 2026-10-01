@ Import veneers after the OVERLAY_66A overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_66A overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
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
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080201b8
	.else
	overlay_veneer Func_080201e8
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
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
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
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020330
	.else
	overlay_veneer Func_08020360
	.endif
	.endif
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001430
	.thumb_func
Func_02001430:
	overlay_veneer Func_080c8018
	.global Func_02001438
	.thumb_func
Func_02001438:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
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
	overlay_veneer Func_080c80f8
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
	overlay_veneer Func_080c8180
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
	overlay_veneer Func_080c81a0
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
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02001508
	.thumb_func
Func_02001508:
	overlay_veneer Func_080c8240
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_SetStatus1c6Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_SetStatus1c6Far
	.else
	overlay_veneer Event_WaitValue1c8FramesFar
	.endif
	.endif
	.global Func_02001520
	.thumb_func
Func_02001520:
	overlay_veneer Func_080c8480
	.global Func_02001528
	.thumb_func
Func_02001528:
	overlay_veneer Func_080c84e0
	.global BattleFx_StartItemBreak
	.thumb_func
BattleFx_StartItemBreak:
	overlay_veneer BattleFx_StartItemBreakFar
	.global BattleFx_SnapScaleToFull
	.thumb_func
BattleFx_SnapScaleToFull:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8520
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8520
	.else
	overlay_veneer BattleFx_SnapScaleToFullFar
	.endif
	.endif
	.global UpdateRisingParticleBurst
	.thumb_func
UpdateRisingParticleBurst:
	overlay_veneer UpdateRisingParticleBurstFar
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
	overlay_veneer Object_SetActionCallbackFar
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
	overlay_veneer Func_080c8778
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
	overlay_veneer Func_080c87f8
	.global Field_BeginPaletteTransition
	.thumb_func
Field_BeginPaletteTransition:
	overlay_veneer Field_BeginPaletteTransitionFar
	.global Func_020015b0
	.thumb_func
Func_020015b0:
	overlay_veneer Func_080c8808
	.global Func_020015b8
	.thumb_func
Func_020015b8:
	overlay_veneer Func_080c8810
	.global Func_020015c0
	.thumb_func
Func_020015c0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8808
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8808
	.else
	overlay_veneer Func_080c8818
	.endif
	.endif
	.global Func_020015c8
	.thumb_func
Func_020015c8:
	overlay_veneer Func_080c8820
	.global Func_020015d0
	.thumb_func
Func_020015d0:
	overlay_veneer Func_080c8840
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
	overlay_veneer Func_080c88a8
	.global Func_020015f0
	.thumb_func
Func_020015f0:
	overlay_veneer Func_080c88b0
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
