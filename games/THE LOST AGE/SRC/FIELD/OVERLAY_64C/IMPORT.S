@ Import veneers after the OVERLAY_64C overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_64C overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	overlay_veneer IwramSignedRemainder
	overlay_veneer IwramUnsignedRemainder
	overlay_veneer WaitFramesFar
	overlay_veneer Func_080000d0
	overlay_veneer Func_080000d8
	overlay_veneer Func_080000f8
	overlay_veneer Math_SineFar
	overlay_veneer Math_CosineFar
	overlay_veneer Func_08000128
	overlay_veneer Func_08000170
	overlay_veneer Sys_FreeFar
	overlay_veneer Func_080001a8
	overlay_veneer Func_080001b8
	overlay_veneer VramBlock_LoadCachedFar
	overlay_veneer Resource_FindFreeEntryFar
	overlay_veneer Func_080001d8
	overlay_veneer Func_080001e8
	overlay_veneer Func_08000268
	overlay_veneer Func_08000298
	overlay_veneer Func_08000318
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Animation_ApplyChildArgumentFar
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetCallback
	overlay_veneer Func_080200c0
	overlay_veneer Func_080200c8
	overlay_veneer Func_08020128
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Animation_SetStateFlagsFar
	overlay_veneer Func_08020228
	overlay_veneer Animation_ApplyChildValuesFar
	overlay_veneer Func_08020350
	overlay_veneer Func_080ad310
	overlay_veneer Func_080ad318
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Object_GetByIdFar
	overlay_veneer Func_080c80f8
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Func_080c8230
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8268
	overlay_veneer Event_SetStatus1c6Far
	overlay_veneer Event_ClearStatus1c6Far
	overlay_veneer Event_WaitValue1c8FramesFar
	overlay_veneer Func_080c8430
	overlay_veneer Func_080c8438
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8638
	overlay_veneer Func_080c86f0
	overlay_veneer Func_080c8778
	overlay_veneer Func_080c8960
	overlay_veneer Func_081a0000
	overlay_veneer Audio_PlayCue
