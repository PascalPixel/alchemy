.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global WaitTicks
	.thumb_func
WaitTicks:
	overlay_veneer WaitTicksFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Resource_DecodeType01
	.thumb_func
Resource_DecodeType01:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_020005ac
	.thumb_func
Func_020005ac:
	overlay_veneer Runtime_PushSlotEntryFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Func_020005bc
	.thumb_func
Func_020005bc:
	overlay_veneer Resource_ReservedNoOpFar
	.global Fade_ToBlack
	.thumb_func
Fade_ToBlack:
	overlay_veneer Fade_ToBlackFar
	.global Fade_Wait
	.thumb_func
Fade_Wait:
	overlay_veneer Fade_WaitFar
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
	overlay_veneer Party_SetLeaderJennaFar
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	overlay_veneer Party_SetLeaderFelixFar
	.global FieldEvent_WaitTicks
	.thumb_func
FieldEvent_WaitTicks:
	overlay_veneer FieldEvent_WaitTicksFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_02000604
	.thumb_func
Func_02000604:
	overlay_veneer Event_SetDestinationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_0200061c
	.thumb_func
Func_0200061c:
	overlay_veneer Resource_FarCall00A + 1
	.global Func_02000624
	.thumb_func
Func_02000624:
	overlay_veneer Resource_FarCall00B + 1
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
