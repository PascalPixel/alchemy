@ Import veneers after the OVERLAY_6B9 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B9 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02000530
	.thumb_func
Func_02000530:
	overlay_veneer Func_080000d0
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Func_02000560
	.thumb_func
Func_02000560:
	overlay_veneer Func_080001a8
	.global Func_02000568
	.thumb_func
Func_02000568:
	overlay_veneer Func_080001b8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Func_02000588
	.thumb_func
Func_02000588:
	overlay_veneer Func_080003c8
	.global Func_02000590
	.thumb_func
Func_02000590:
	overlay_veneer Func_080003d0
	.global Func_02000598
	.thumb_func
Func_02000598:
	overlay_veneer Func_080200c0
	.global Func_020005a0
	.thumb_func
Func_020005a0:
	overlay_veneer Func_080201e0
	.global Func_020005a8
	.thumb_func
Func_020005a8:
	overlay_veneer Func_080201e8
	.global Func_020005b0
	.thumb_func
Func_020005b0:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020005c8
	.thumb_func
Func_020005c8:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_020005d8
	.thumb_func
Func_020005d8:
	overlay_veneer Func_080c8280
	.global Func_020005e0
	.thumb_func
Func_020005e0:
	overlay_veneer Func_080c86a8
	.global Func_020005e8
	.thumb_func
Func_020005e8:
	overlay_veneer Func_080c86b8
	.global Func_020005f0
	.thumb_func
Func_020005f0:
	overlay_veneer Func_080c87c0
	.global Func_020005f8
	.thumb_func
Func_020005f8:
	overlay_veneer Func_080c87d0
	.global Func_02000600
	.thumb_func
Func_02000600:
	overlay_veneer Func_080c87e0
