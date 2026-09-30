@ Import veneers after the OVERLAY_6A7 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6A7 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	overlay_veneer IwramSignedRemainder
	overlay_veneer WaitFramesFar
	overlay_veneer Func_080000f8
	overlay_veneer Func_08000140
	overlay_veneer Runtime_ReleaseHeapBlockFar
	overlay_veneer Runtime_BumpAllocateFar
	overlay_veneer Func_08000170
	overlay_veneer Sys_FreeFar
	overlay_veneer Func_080001a8
	overlay_veneer Func_080001b8
	overlay_veneer VramBlock_LoadCachedFar
	overlay_veneer Resource_FindFreeEntryFar
	overlay_veneer Func_080001e8
	overlay_veneer Resource_GetTableEntryFar
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Object_CommitPosition
	overlay_veneer Func_080201e8
	overlay_veneer Func_080201f0
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08020228
	overlay_veneer Animation_ApplyChildValuesFar
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8070
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	overlay_veneer Func_080c80f8
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	overlay_veneer Func_080c8198
	overlay_veneer Func_080c81d8
	overlay_veneer Func_080c8228
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8268
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c8330
	overlay_veneer Func_080c8378
	overlay_veneer Func_080c8390
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c84e0
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	overlay_veneer Func_080c87c0
	overlay_veneer Func_080c87d0
	overlay_veneer Func_080c87e0
	overlay_veneer Audio_PlayCue
