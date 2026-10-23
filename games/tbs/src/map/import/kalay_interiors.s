@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_MapGridAnimateCells, 1
	.set import.Engine_MapGridCopyCellsTo, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Engine_ScriptWait, 1
	.set import.Engine_ScriptStart, 1
	.set import.Engine_ScriptEnd, 1
	.set import.Map_PrepareObjectEvents, 1
	.set import.ObjectEvent_GetById, 1
	.set import.Engine_ObjectEventDestroy, 1
	.set import.Engine_ObjectEventSetSpeed, 1
	.set import.Engine_ObjectEventCenterAndWalk, 1
	.set import.Engine_ObjectEventSetAnimation, 1
	.set import.Engine_ObjectEventSetChildValue, 1
	.set import.Engine_ScriptSetMessage, 1
	.set import.Engine_ScriptShowMessage, 1
	.set import.Engine_ScriptAskYesNo, 1
	.set import.Engine_ScriptRequestWarp, 1
	.set import.Engine_ScriptCloseScreen, 1
	.set import.Engine_ScriptWaitForScreen, 1
	.set import.Engine_SanctumOpen, 1
	.set import.Engine_InnOpen, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
