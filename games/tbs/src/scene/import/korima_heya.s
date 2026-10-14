@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_MapCopyCellsTo, 1
	.set import.Engine_ActorSetSpriteFlags, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_EventBegin, 1
	.set import.Engine_EventEnd, 1
	.set import.FieldScene_PrepareActors, 1
	.set import.Object_GetById, 1
	.set import.Engine_ActorSetPosition, 1
	.set import.Engine_EventSetMessage, 1
	.set import.Engine_EventShowMessage, 1
	.set import.Engine_ShopOpen, 1
	.set import.Engine_InnOpen, 1
	.include "games/tbs/src/system/overlay.inc"
