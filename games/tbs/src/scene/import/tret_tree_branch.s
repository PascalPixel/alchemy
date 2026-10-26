@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Task_Add, 1
	.set import.Irq_SetHandler, 1
	.set import.Serial_PollAndTransfer, 1
	.set import.MapGrid_GetTerrainHeight, 1
	.set import.FieldScript_Start, 1
	.set import.FieldScript_End, 1
	.set import.ObjectEvent_GetById, 1
	.set import.Import_ScriptRequestWarp, 1
	.set import.Field_RequestDjinnBattle, 1
	.set import.ScreenTransition_InitializeBattleEffectState, 1
	.set import.Sound_PlaySong, 1
	.include "games/tbs/src/system/overlay.inc"
