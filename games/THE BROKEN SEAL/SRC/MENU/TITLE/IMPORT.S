@ Import veneers after the TITLE overlay's code: fixed 8-byte veneers through
@ which the overlay calls main-image code, each loading its target into r4 and
@ branching, which the calling convention permits.
@ credit: reconstructed_veneer — TITLE overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global Sys_Free
	.thumb_func
Sys_Free:
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
	.global Runtime_PushSlotEntry
	.thumb_func
Runtime_PushSlotEntry:
	overlay_veneer Runtime_PushSlotEntryFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global RuntimeDispatch_NoOpHook
	.thumb_func
RuntimeDispatch_NoOpHook:
	overlay_veneer RuntimeDispatch_NoOpHookFar
	.global Blend_SetDarkenTarget16
	.thumb_func
Blend_SetDarkenTarget16:
	overlay_veneer Blend_SetDarkenTarget16Far
	.global Blend_WaitForTransition
	.thumb_func
Blend_WaitForTransition:
	overlay_veneer Blend_WaitForTransitionFar
	.global SaveState_ScanRecordFlags
	.thumb_func
SaveState_ScanRecordFlags:
	overlay_veneer SaveState_ScanRecordFlagsFar
	.global Party_ApplyStatePreset
	.thumb_func
Party_ApplyStatePreset:
	overlay_veneer Party_ApplyStatePresetFar
	.global Engine_EventWait
	.thumb_func
Engine_EventWait:
	overlay_veneer Battle_WaitMode0Far
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Event_SetPairWork1c0
	.thumb_func
Event_SetPairWork1c0:
	overlay_veneer Event_SetPairWork1c0Far
	.global Engine_EventOpenScreen
	.thumb_func
Engine_EventOpenScreen:
	overlay_veneer Event_SetStatus1c6Far
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global ScrollFar_Entry00
	.thumb_func
ScrollFar_Entry00:
	overlay_veneer Func_080f0000
	.global PaletteFar_Entry00
	.thumb_func
PaletteFar_Entry00:
	overlay_veneer Func_080f2000
	.global Title_ShowSplashScreen
	.thumb_func
Title_ShowSplashScreen:
	overlay_veneer Title_ShowSplashScreenFar
	.global PaletteFar_Entry20
	.thumb_func
PaletteFar_Entry20:
	overlay_veneer Title_ShowAnimatedSplashFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
