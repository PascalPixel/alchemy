@ Import veneers after the OVERLAY_649 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_649 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02000594
	.thumb_func
Func_02000594:
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_020005ac
	.thumb_func
Func_020005ac:
	overlay_veneer Func_080001e8
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Func_020005bc
	.thumb_func
Func_020005bc:
	overlay_veneer Func_08000298
	.global Blend_SetDarkenTarget16
	.thumb_func
Blend_SetDarkenTarget16:
	overlay_veneer Blend_SetDarkenTarget16Far
	.global Blend_WaitForTransition
	.thumb_func
Blend_WaitForTransition:
	overlay_veneer Blend_WaitForTransitionFar
	.global Func_020005d4
	.thumb_func
Func_020005d4:
	overlay_veneer Func_080382f0
	.global Func_020005dc
	.thumb_func
Func_020005dc:
	overlay_veneer Func_08038348
	.global Func_020005e4
	.thumb_func
Func_020005e4:
	overlay_veneer Func_080ad218
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	overlay_veneer Func_080ad220
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_02000604
	.thumb_func
Func_02000604:
	overlay_veneer Func_080c8268
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_0200061c
	.thumb_func
Func_0200061c:
	overlay_veneer Func_081a0000
	.global Func_02000624
	.thumb_func
Func_02000624:
	overlay_veneer Func_081a6000
	.global Func_0200062c
	.thumb_func
Func_0200062c:
	overlay_veneer Func_081a6018
	.global Func_02000634
	.thumb_func
Func_02000634:
	overlay_veneer Func_081a6020
	.global Func_0200063c
	.thumb_func
Func_0200063c:
	overlay_veneer Func_081a6028
	.global Func_02000644
	.thumb_func
Func_02000644:
	overlay_veneer Audio_PlayCue
