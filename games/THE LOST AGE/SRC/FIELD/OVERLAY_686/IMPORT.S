@ Import veneers after the OVERLAY_686 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_686 overlay import veneers
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
	overlay_veneer Runtime_BumpAllocateFar
	overlay_veneer Sys_FreeFar
	overlay_veneer VramBlock_LoadCachedFar
	overlay_veneer Resource_FindFreeEntryFar
	overlay_veneer Func_080001e8
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Func_08000418
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetCallback
	overlay_veneer Func_080200c0
	overlay_veneer Func_08020120
	overlay_veneer Func_080201e0
	overlay_veneer Func_080201e8
	overlay_veneer Func_080201f0
	overlay_veneer Func_08020210
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Animation_SetStateFlagsFar
	overlay_veneer Func_08020228
	overlay_veneer Func_08020230
	overlay_veneer Func_08020290
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	overlay_veneer Func_080c80f8
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Func_080c8150
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8218
	overlay_veneer Func_080c8228
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c8480
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8628
	overlay_veneer Audio_PlayCue
