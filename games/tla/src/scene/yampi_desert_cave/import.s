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
	overlay_veneer Map_CopyCellAttributeRectFar
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
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001870
	.thumb_func
Func_02001870:
	overlay_veneer EventRuntime_StartFar
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
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_020018e8
	.thumb_func
Func_020018e8:
	overlay_veneer Func_080c8288
	.global Func_020018f0
	.thumb_func
Func_020018f0:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_020018f8
	.thumb_func
Func_020018f8:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02001900
	.thumb_func
Func_02001900:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Object_SetWideSprite
	.thumb_func
Object_SetWideSprite:
	overlay_veneer Object_SetWideSpriteFar
	.global Func_02001920
	.thumb_func
Func_02001920:
	overlay_veneer Object_ReturnHomeFar
	.global Func_02001928
	.thumb_func
Func_02001928:
	overlay_veneer Func_080c8480
	.global Func_02001930
	.thumb_func
Func_02001930:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02001938
	.thumb_func
Func_02001938:
	overlay_veneer Event_NoOpFar
	.global Func_02001940
	.thumb_func
Func_02001940:
	overlay_veneer Func_080c8710
	.global Func_02001948
	.thumb_func
Func_02001948:
	overlay_veneer Func_080c8738
	.global Func_02001950
	.thumb_func
Func_02001950:
	overlay_veneer Func_080c88d8
	.global Func_02001958
	.thumb_func
Func_02001958:
	overlay_veneer Func_080c88e8
	.global Func_02001960
	.thumb_func
Func_02001960:
	overlay_veneer Sound_PlayFar
