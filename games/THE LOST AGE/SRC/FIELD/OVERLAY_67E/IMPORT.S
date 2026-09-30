@ Import veneers after the OVERLAY_67E overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_67E overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080000d0
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Func_08020178
	overlay_veneer Func_080201e8
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8070
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	overlay_veneer Func_080c80f8
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	overlay_veneer Func_080c8158
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8218
	overlay_veneer Func_080c8228
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8248
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b0
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c84e0
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	overlay_veneer Func_080c8718
	overlay_veneer Func_080c8720
	overlay_veneer Func_080c8728
	overlay_veneer Func_080c8760
	overlay_veneer Func_080c8778
	overlay_veneer Audio_PlayCue
