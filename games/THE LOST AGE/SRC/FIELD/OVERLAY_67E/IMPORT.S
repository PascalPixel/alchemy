@ Import veneers after the OVERLAY_67E overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_67E overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_02000cc0
	.thumb_func
Func_02000cc0:
	overlay_veneer Func_080000d0
	.global Func_02000cc8
	.thumb_func
Func_02000cc8:
	overlay_veneer Func_080003c8
	.global Func_02000cd0
	.thumb_func
Func_02000cd0:
	overlay_veneer Func_080003d0
	.global Func_02000cd8
	.thumb_func
Func_02000cd8:
	overlay_veneer Func_080003d8
	.global Func_02000ce0
	.thumb_func
Func_02000ce0:
	overlay_veneer Func_08020178
	.global Func_02000ce8
	.thumb_func
Func_02000ce8:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02000d00
	.thumb_func
Func_02000d00:
	overlay_veneer Func_080c8018
	.global Func_02000d08
	.thumb_func
Func_02000d08:
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
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02000d40
	.thumb_func
Func_02000d40:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Func_02000d58
	.thumb_func
Func_02000d58:
	overlay_veneer Func_080c8158
	.global Func_02000d60
	.thumb_func
Func_02000d60:
	overlay_veneer Func_080c8180
	.global Func_02000d68
	.thumb_func
Func_02000d68:
	overlay_veneer Func_080c8188
	.global Func_02000d70
	.thumb_func
Func_02000d70:
	overlay_veneer Func_080c81a0
	.global Func_02000d78
	.thumb_func
Func_02000d78:
	overlay_veneer Func_080c81d0
	.global Func_02000d80
	.thumb_func
Func_02000d80:
	overlay_veneer Func_080c8200
	.global Func_02000d88
	.thumb_func
Func_02000d88:
	overlay_veneer Func_080c8210
	.global Func_02000d90
	.thumb_func
Func_02000d90:
	overlay_veneer Func_080c8218
	.global Func_02000d98
	.thumb_func
Func_02000d98:
	overlay_veneer Func_080c8228
	.global Func_02000da0
	.thumb_func
Func_02000da0:
	overlay_veneer Func_080c8240
	.global Func_02000da8
	.thumb_func
Func_02000da8:
	overlay_veneer Func_080c8248
	.global Func_02000db0
	.thumb_func
Func_02000db0:
	overlay_veneer Func_080c8278
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02000dd0
	.thumb_func
Func_02000dd0:
	overlay_veneer Func_080c84e0
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_02000de0
	.thumb_func
Func_02000de0:
	overlay_veneer Func_080c8718
	.global Func_02000de8
	.thumb_func
Func_02000de8:
	overlay_veneer Func_080c8720
	.global Func_02000df0
	.thumb_func
Func_02000df0:
	overlay_veneer Func_080c8728
	.global Func_02000df8
	.thumb_func
Func_02000df8:
	overlay_veneer Func_080c8760
	.global Func_02000e00
	.thumb_func
Func_02000e00:
	overlay_veneer Func_080c8778
	.global Func_02000e08
	.thumb_func
Func_02000e08:
	overlay_veneer Audio_PlayCue
