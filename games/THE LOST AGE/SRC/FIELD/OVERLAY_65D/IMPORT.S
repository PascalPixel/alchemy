@ Import veneers after the OVERLAY_65D overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_65D overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_02000600
	.thumb_func
Func_02000600:
	overlay_veneer Func_080003c8
	.global Func_02000608
	.thumb_func
Func_02000608:
	overlay_veneer Func_080003d0
	.global Func_02000610
	.thumb_func
Func_02000610:
	overlay_veneer Func_08020170
	.global Func_02000618
	.thumb_func
Func_02000618:
	overlay_veneer Func_08020178
	.global Func_02000620
	.thumb_func
Func_02000620:
	overlay_veneer Func_080201a0
	.global Func_02000628
	.thumb_func
Func_02000628:
	overlay_veneer Func_080201e0
	.global Func_02000630
	.thumb_func
Func_02000630:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02000648
	.thumb_func
Func_02000648:
	overlay_veneer Func_080c8018
	.global Func_02000650
	.thumb_func
Func_02000650:
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
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_SnapHeadingAndOffset
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	overlay_veneer ObjectMotion_SnapHeadingAndOffsetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02000698
	.thumb_func
Func_02000698:
	overlay_veneer Func_080c8180
	.global Func_020006a0
	.thumb_func
Func_020006a0:
	overlay_veneer Func_080c8188
	.global Func_020006a8
	.thumb_func
Func_020006a8:
	overlay_veneer Func_080c81a0
	.global Func_020006b0
	.thumb_func
Func_020006b0:
	overlay_veneer Func_080c81d0
	.global Func_020006b8
	.thumb_func
Func_020006b8:
	overlay_veneer Func_080c8278
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_020006d0
	.thumb_func
Func_020006d0:
	overlay_veneer Func_080c84e0
	.global Func_020006d8
	.thumb_func
Func_020006d8:
	overlay_veneer Func_080c8580
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_020006e8
	.thumb_func
Func_020006e8:
	overlay_veneer Func_080c8778
	.global Func_020006f0
	.thumb_func
Func_020006f0:
	overlay_veneer Audio_PlayCue
