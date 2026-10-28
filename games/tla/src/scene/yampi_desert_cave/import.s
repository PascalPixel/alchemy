.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
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
	overlay_veneer Actor_SetMode
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02001838
	.thumb_func
Func_02001838:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02001840
	.thumb_func
Func_02001840:
	overlay_veneer Actor_CommitPosition
	.global Func_02001848
	.thumb_func
Func_02001848:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02001850
	.thumb_func
Func_02001850:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02001860
	.thumb_func
Func_02001860:
	overlay_veneer Func_08038210
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02001870
	.thumb_func
Func_02001870:
	overlay_veneer Event_StartFar
	.global Func_02001878
	.thumb_func
Func_02001878:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_020018a8
	.thumb_func
Func_020018a8:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
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
	.global Actor_SetWideSprite
	.thumb_func
Actor_SetWideSprite:
	overlay_veneer Actor_SetWideSpriteFar
	.global Func_02001920
	.thumb_func
Func_02001920:
	overlay_veneer Actor_ReturnHomeFar
	.global Func_02001928
	.thumb_func
Func_02001928:
	overlay_veneer Func_080c8480
	.global Func_02001930
	.thumb_func
Func_02001930:
	overlay_veneer Event_ResolvePendingActionsFar
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
