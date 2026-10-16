.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
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
	.ifndef TLA_EDITION_JA
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.endif
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_02001828
	.thumb_func
Func_02001828:
	overlay_veneer Object_SetMode
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02001838
	.thumb_func
Func_02001838:
	overlay_veneer Object_SetPosition
	.global Func_02001840
	.thumb_func
Func_02001840:
	overlay_veneer Object_CommitPosition
	.global Func_02001848
	.thumb_func
Func_02001848:
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
	.global Func_02001850
	.thumb_func
Func_02001850:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02001860
	.thumb_func
Func_02001860:
	overlay_veneer Func_08038210
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001870
	.thumb_func
Func_02001870:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02001878
	.thumb_func
Func_02001878:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_020018a8
	.thumb_func
Func_020018a8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_020018c8
	.thumb_func
Func_020018c8:
	overlay_veneer Func_080c8208
	.global Func_020018d0
	.thumb_func
Func_020018d0:
	overlay_veneer Func_080c8210
	.global Func_020018d8
	.thumb_func
Func_020018d8:
	overlay_veneer Event_SetDestinationFar
	.global Func_020018e0
	.thumb_func
Func_020018e0:
	overlay_veneer BattleFx_SetWeightedResultFar
	.global Func_020018e8
	.thumb_func
Func_020018e8:
	overlay_veneer Func_080c8288
	.global Func_020018f0
	.thumb_func
Func_020018f0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8368
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8368
	.else
	overlay_veneer BattleFx_ApplyColorToTargetBufferFar
	.endif
	.endif
	.global Func_020018f8
	.thumb_func
Func_020018f8:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	overlay_veneer BattleFx_ApplyColorToSourceBufferFar
	.endif
	.endif
	.global Func_02001900
	.thumb_func
Func_02001900:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_ApplyColorToSourceBufferFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_ApplyColorToSourceBufferFar
	.else
	overlay_veneer BattleFx_StartBufferInterpolationFar
	.endif
	.endif
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
	.global Object_SetWideSprite
	.thumb_func
Object_SetWideSprite:
	.ifdef TLA_EDITION_ES
	overlay_veneer ObjectEvent_RemoveFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer ObjectEvent_RemoveFar
	.else
	overlay_veneer Object_SetWideSpriteFar
	.endif
	.endif
	.global Func_02001920
	.thumb_func
Func_02001920:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8418
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8418
	.else
	overlay_veneer Object_ReturnHomeFar
	.endif
	.endif
	.global Func_02001928
	.thumb_func
Func_02001928:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global Func_02001930
	.thumb_func
Func_02001930:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02001938
	.thumb_func
Func_02001938:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_SetActionCallbackFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_SetActionCallbackFar
	.else
	overlay_veneer Event_NoOpFar
	.endif
	.endif
	.global Func_02001940
	.thumb_func
Func_02001940:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_02001948
	.thumb_func
Func_02001948:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8728
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8728
	.else
	overlay_veneer Func_080c8738
	.endif
	.endif
	.global Func_02001950
	.thumb_func
Func_02001950:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88c8
	.else
	overlay_veneer Func_080c88d8
	.endif
	.endif
	.global Func_02001958
	.thumb_func
Func_02001958:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88d8
	.else
	overlay_veneer Func_080c88e8
	.endif
	.endif
	.global Func_02001960
	.thumb_func
Func_02001960:
	overlay_veneer Audio_PlayCue
