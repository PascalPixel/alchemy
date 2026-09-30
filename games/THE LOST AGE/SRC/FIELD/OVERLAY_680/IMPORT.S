@ Import veneers after the OVERLAY_680 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_680 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer WaitFramesFar
	overlay_veneer Func_080000d0
	overlay_veneer Func_080000f8
	overlay_veneer Math_SineFar
	overlay_veneer Math_CosineFar
	overlay_veneer Func_08000128
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetCallback
	overlay_veneer Func_080200c0
	overlay_veneer Object_SetPosition
	overlay_veneer Func_080201e8
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08020228
	overlay_veneer Func_08020290
	overlay_veneer Func_08038040
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Object_GetByIdFar
	overlay_veneer Func_080c80f8
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81a8
	overlay_veneer Func_080c8218
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8248
	overlay_veneer Func_080c8368
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8758
	overlay_veneer Func_080c8760
	overlay_veneer Func_080c8778
	overlay_veneer Func_080c8780
	overlay_veneer Func_080c88c8
	overlay_veneer Audio_PlayCue
