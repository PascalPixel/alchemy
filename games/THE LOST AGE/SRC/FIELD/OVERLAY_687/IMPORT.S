@ Import veneers after the OVERLAY_687 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_687 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	overlay_veneer WaitFramesFar
	overlay_veneer Func_080000d0
	overlay_veneer Func_080000d8
	overlay_veneer Func_080000f8
	overlay_veneer Math_SineFar
	overlay_veneer Math_CosineFar
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetCallback
	overlay_veneer Func_080200c0
	overlay_veneer Func_080200c8
	overlay_veneer Object_SetPosition
	overlay_veneer Func_08020178
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Animation_SetStateFlagsFar
	overlay_veneer Func_08020228
	overlay_veneer Func_08020230
	overlay_veneer Func_080202f8
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c8230
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c8280
	overlay_veneer Func_080c8290
	overlay_veneer Func_080c8378
	overlay_veneer Func_080c8380
	overlay_veneer Event_SetStatus1c6Far
	overlay_veneer Event_ClearStatus1c6Far
	overlay_veneer Event_WaitValue1c8FramesFar
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8570
	overlay_veneer Audio_PlayCue
