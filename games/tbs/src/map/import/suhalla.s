@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_MapAnimateCells, 1
	.set import.Engine_MapCopyCellAttributes, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Engine_EventWait, 1
	.set import.Engine_EventChooseYesNo, 1
	.set import.Engine_ActorSetAnimation, 1
	.set import.Engine_EventSetMessage, 1
	.set import.Engine_EventOpenMessage, 1
	.set import.Engine_EventShowMessage, 1
	.set import.Engine_EventRequestExit, 1
	.set import.Engine_ActorWalkBy, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
