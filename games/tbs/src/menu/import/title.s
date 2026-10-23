@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_TaskWait, 1
	.set import.Scheduler_AddOrUpdateCallback, 1
	.set import.Runtime_BumpAllocateAlternatePool, 1
	.set import.MemFree, 1
	.set import.Resource_DecodeType01, 1
	.set import.Engine_VramLoad, 1
	.set import.Resource_FindFreeEntry, 1
	.set import.Runtime_PushSlotEntry, 1
	.set import.Resource_GetTableEntry, 1
	.set import.RuntimeDispatch_NoOpHook, 1
	.set import.Fade_ToBlack, 1
	.set import.Fade_Wait, 1
	.set import.SaveState_ScanRecordFlags, 1
	.set import.Party_EndChildhood, 1
	.set import.Engine_ScriptWait, 1
	.set import.ObjectEvent_GetById, 1
	.set import.FieldScript_SetDestination, 1
	.set import.Engine_ScriptOpenScreen, 1
	.set import.Engine_ScriptWaitForScreen, 1
	.set import.ScrollFar_Entry00, 1
	.set import.PaletteFar_Entry00, 1
	.set import.Title_ShowSplashScreen, 1
	.set import.PaletteFar_Entry20, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
