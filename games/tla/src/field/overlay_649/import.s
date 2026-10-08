.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
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
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Resource_DecodeType01
	.thumb_func
Resource_DecodeType01:
	overlay_veneer Resource_DecodeType01Far
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
	overlay_veneer Runtime_PushSlotEntryFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Func_020005bc
	.thumb_func
Func_020005bc:
	overlay_veneer Resource_ReservedNoOpFar
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
	overlay_veneer Party_SetLeaderJasmineFar
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	overlay_veneer Party_SetLeaderGarciaFar
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
	overlay_veneer Event_SetDestinationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer Runtime_ScheduleCallbackAndReleaseBlock32BFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Runtime_ScheduleCallbackAndReleaseBlock32BFar
	.else
	overlay_veneer Event_StartScreenTransitionFar
	.endif
	.endif
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_DelayEffectFramesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_DelayEffectFramesFar
	.else
	overlay_veneer Event_WaitTransitionFramesFar
	.endif
	.endif
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
