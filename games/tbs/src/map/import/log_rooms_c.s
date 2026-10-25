@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.MapGrid_CopyMetatileIndicesRect, 1
	.set import.Object_SetSpriteFlags, 1
	.set import.GameFlag_Get, 1
	.set import.FieldScript_Start, 1
	.set import.FieldScript_End, 1
	.set import.Map_AssignViewFlags, 1
	.set import.ObjectEvent_GetById, 1
	.set import.Import_ObjectEventSetPosition, 1
	.set import.FieldScript_SetMessageId, 1
	.set import.FieldScript_ShowMessageAndWait, 1
	.set import.Shop_Run, 1
	.set import.Inn_CheckIn, 1
	.include "games/tbs/src/system/overlay.inc"
