@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.MapGrid_PlayMetatileCopySequence, 1
	.set import.MapGrid_CopyMetatileIndicesRect, 1
	.set import.GameFlag_Get, 1
	.set import.GameFlag_Set, 1
	.set import.FieldScript_WaitFrames, 1
	.set import.FieldScript_Start, 1
	.set import.FieldScript_End, 1
	.set import.Map_AssignViewFlags, 1
	.set import.ObjectEvent_GetById, 1
	.set import.ObjectEvent_DestroyById, 1
	.set import.ObjectMotion_SetSpeedParameters, 1
	.set import.ObjectMotion_CenterOnColumn, 1
	.set import.Import_ObjectEventSetAnimation, 1
	.set import.ObjectEvent_ConfigureSpriteColor, 1
	.set import.FieldScript_SetMessageId, 1
	.set import.FieldScript_ShowMessageAndWait, 1
	.set import.FieldScript_AskYesNoAndShowResponse, 1
	.set import.Import_ScriptRequestWarp, 1
	.set import.FieldScript_FinishScreenTransition, 1
	.set import.FieldScript_WaitTransitionFrames, 1
	.set import.Sanctum_Run, 1
	.set import.Inn_CheckIn, 1
	.set import.Sound_PlaySong, 1
	.include "games/tbs/src/system/overlay.inc"
