@ Import veneers after the OVERLAY_662 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_662 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
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
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
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
	overlay_veneer Func_080201e8
	.global Func_02001850
	.thumb_func
Func_02001850:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
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
	overlay_veneer Func_080c8018
	.global Func_02001878
	.thumb_func
Func_02001878:
	overlay_veneer Func_080c8020
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
	overlay_veneer Func_080c80f8
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
	overlay_veneer Event_SetPairWork1c0Far
	.global Func_020018e0
	.thumb_func
Func_020018e0:
	overlay_veneer Func_080c8280
	.global Func_020018e8
	.thumb_func
Func_020018e8:
	overlay_veneer Func_080c8288
	.global Func_020018f0
	.thumb_func
Func_020018f0:
	overlay_veneer Func_080c8378
	.global Func_020018f8
	.thumb_func
Func_020018f8:
	overlay_veneer Func_080c8380
	.global Func_02001900
	.thumb_func
Func_02001900:
	overlay_veneer Func_080c8390
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Object_SetWideSprite
	.thumb_func
Object_SetWideSprite:
	overlay_veneer Object_SetWideSpriteFar
	.global Func_02001920
	.thumb_func
Func_02001920:
	overlay_veneer Func_080c8428
	.global Func_02001928
	.thumb_func
Func_02001928:
	overlay_veneer Func_080c8480
	.global Func_02001930
	.thumb_func
Func_02001930:
	overlay_veneer Func_080c84e0
	.global Func_02001938
	.thumb_func
Func_02001938:
	overlay_veneer Func_080c8690
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
	overlay_veneer Audio_PlayCue
