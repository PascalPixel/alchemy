@ Import veneers after the OVERLAY_6B1 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B1 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	overlay_veneer IwramUnsignedRemainder
	overlay_veneer WaitFramesFar
	overlay_veneer Func_080000d0
	overlay_veneer Func_080000f8
	overlay_veneer Runtime_BumpAllocateFar
	overlay_veneer Sys_FreeFar
	overlay_veneer Func_080001a8
	overlay_veneer VramBlock_LoadCachedFar
	overlay_veneer Resource_FindFreeEntryFar
	overlay_veneer Func_080001e8
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetCallback
	overlay_veneer Func_080200c0
	overlay_veneer Object_SetPosition
	overlay_veneer Object_CommitPosition
	overlay_veneer Func_080201c0
	overlay_veneer Func_080201c8
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Animation_SetStateFlagsFar
	overlay_veneer Func_08038210
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	overlay_veneer Object_SetActionCallbackAndRefreshByIdFar
	overlay_veneer Func_080c8110
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c8280
	overlay_veneer Func_080c82c0
	overlay_veneer Func_080c82e0
	overlay_veneer Func_080c8378
	overlay_veneer Func_080c8380
	overlay_veneer Func_080c8390
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8740
	overlay_veneer Func_080c8748
	overlay_veneer Func_080c87e8
	overlay_veneer Audio_PlayCue
