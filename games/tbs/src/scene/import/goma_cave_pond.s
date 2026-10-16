@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_MapCopyCellAttributes, 1
	.set import.Engine_ActorSetSpriteFlags, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Object_GetById, 1
	.set import.Engine_ActorSetPosition, 1
	.set import.Engine_ActorSetAnimation, 1
	.include "games/tbs/src/system/overlay.inc"
