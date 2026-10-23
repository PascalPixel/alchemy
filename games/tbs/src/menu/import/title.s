@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.WaitFrames, 1
	.set import.Task_Add, 1
	.set import.AllocEwram, 1
	.set import.Free, 1
	.set import.Decompress_Type01, 1
	.set import.VramCache_Load, 1
	.set import.VramCache_FindFreeEntry, 1
	.set import.Oam_AddSpriteAtDepth, 1
	.set import.Resource_GetTableEntry, 1
	.set import.Stub_NoOpHook, 1
	.set import.ScreenFade_ToBlack, 1
	.set import.ScreenFade_Wait, 1
	.set import.Save_ScanRecordFlags, 1
	.set import.Party_EndChildhood, 1
	.set import.FieldScript_WaitFrames, 1
	.set import.ObjectEvent_GetById, 1
	.set import.FieldScript_SetDestination, 1
	.set import.FieldScript_StartScreenTransition, 1
	.set import.FieldScript_WaitTransitionFrames, 1
	.set import.CreditsFar_Entry00, 1
	.set import.Title_ShowIntro, 1
	.set import.Title_ShowNintendoLogo, 1
	.set import.TitleFar_Entry20, 1
	.set import.Sound_PlaySong, 1
	.include "games/tbs/src/system/overlay.inc"
