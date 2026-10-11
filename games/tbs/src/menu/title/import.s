@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import Engine_TaskWait, WaitFramesFar
	import Engine_TaskAddCallback, Scheduler_AddOrUpdateCallbackFar
	import Runtime_BumpAllocateAlternatePool, Runtime_BumpAllocateAlternatePoolFar
	import MemFree, Sys_FreeFar
	import Resource_DecodeType01, Resource_DecodeType01Far
	import Engine_VramLoad, VramBlock_LoadCachedFar
	import Resource_FindFreeEntry, Resource_FindFreeEntryFar
	import Runtime_PushSlotEntry, Runtime_PushSlotEntryFar
	import Resource_GetTableEntry, Resource_GetTableEntryFar
	import RuntimeDispatch_NoOpHook, RuntimeDispatch_NoOpHookFar
	import Blend_SetDarkenTarget16, Blend_SetDarkenTarget16Far
	import Blend_WaitForTransition, Blend_WaitForTransitionFar
	import SaveState_ScanRecordFlags, SaveState_ScanRecordFlagsFar
	import Party_ApplyStatePreset, Party_ApplyStatePresetFar
	import Engine_EventWait, Battle_WaitMode0Far
	import Object_GetById, Object_GetByIdFar
	import Event_SetDestination, Event_SetDestinationFar
	import Engine_EventOpenScreen, Event_StartScreenTransitionFar
	import Engine_EventWaitForScreen, Event_WaitTransitionFramesFar
	import ScrollFar_Entry00, DisplayScroll_RunSlideshowFar
	import PaletteFar_Entry00, Title_ShowIntroFar
	import Title_ShowSplashScreen, Title_ShowSplashScreenFar
	import PaletteFar_Entry20, Title_ShowAnimatedSplashFar
	import Engine_AudioPlayCue, AudioCommand_PlayFar
