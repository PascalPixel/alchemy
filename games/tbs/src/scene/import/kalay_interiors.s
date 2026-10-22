@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_MapAnimateCells, 1
	.set import.Engine_MapCopyCellsTo, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Engine_EventWait, 1
	.set import.Engine_EventStart, 1
	.set import.Engine_EventEnd, 1
	.set import.FieldScene_PrepareActors, 1
	.set import.Object_GetById, 1
	.set import.Engine_ActorDestroy, 1
	.set import.Engine_ActorSetSpeed, 1
	.set import.Engine_ActorCenterAndWalk, 1
	.set import.Engine_ActorSetAnimation, 1
	.set import.Engine_ActorSetChildValue, 1
	.set import.Engine_EventSetMessage, 1
	.set import.Engine_EventShowMessage, 1
	.set import.Engine_EventAskYesNo, 1
	.set import.Engine_EventRequestExit, 1
	.set import.Engine_EventCloseScreen, 1
	.set import.Engine_EventWaitForScreen, 1
	.set import.Engine_SanctumOpen, 1
	.set import.Engine_InnOpen, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
