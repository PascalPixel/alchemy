@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_MapGridAnimateCells, 1
	.set import.Engine_MapGridCopyCellAttributes, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Engine_ScriptWait, 1
	.set import.Engine_ScriptChooseYesNo, 1
	.set import.Engine_ObjectEventSetAnimation, 1
	.set import.Engine_ScriptSetMessage, 1
	.set import.Engine_ScriptOpenMessage, 1
	.set import.Engine_ScriptShowMessage, 1
	.set import.Engine_ScriptRequestWarp, 1
	.set import.Engine_ObjectEventWalkBy, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
