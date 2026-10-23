@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.MapGrid_CopyCellAttributeRect, 1
	.set import.ObjectDispatch_SetSingleChildFlags, 1
	.set import.GameFlag_Get, 1
	.set import.ObjectEvent_GetById, 1
	.set import.Engine_ObjectEventSetPosition, 1
	.set import.Engine_ObjectEventSetAnimation, 1
	.include "games/tbs/src/system/overlay.inc"
