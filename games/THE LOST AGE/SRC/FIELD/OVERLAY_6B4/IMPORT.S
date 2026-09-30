@ Import veneers after the OVERLAY_6B4 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B4 overlay import veneers
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
	overlay_veneer Func_08000170
	overlay_veneer Sys_FreeFar
	overlay_veneer Func_080001f0
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetCallback
	overlay_veneer Func_080200c0
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08020228
	overlay_veneer Func_08038348
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	overlay_veneer Func_080c80f8
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Object_SetActionByIdFar
	overlay_veneer Motion_SetVarCbAndRefreshFar
	overlay_veneer Func_080c8168
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8198
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8218
	overlay_veneer Func_080c8230
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8268
	overlay_veneer Func_080c8378
	overlay_veneer Func_080c8390
	overlay_veneer Event_SetStatus1c6Far
	overlay_veneer Event_ClearStatus1c6Far
	overlay_veneer Event_WaitValue1c8FramesFar
	overlay_veneer Func_080c87f8
	overlay_veneer Field_BeginPaletteTransitionFar
	overlay_veneer Func_080c8808
	overlay_veneer Func_080c8810
	overlay_veneer Func_080c8818
	overlay_veneer Func_080c8820
	overlay_veneer Audio_PlayCue
