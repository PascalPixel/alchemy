@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_GameFlagClear, 1
	.set import.Engine_ScriptWait, 1
	.set import.Engine_ScriptStart, 1
	.set import.Engine_ScriptEnd, 1
	.set import.ObjectEvent_GetById, 1
	.set import.Engine_ObjectEventSetAnimationAndWait, 1
	.set import.Engine_ObjectEventStartRepeatedMotion, 1
	.set import.Engine_ObjectEventRunRepeatedMotion, 1
	.set import.Engine_ObjectEventFaceObjectEvent, 1
	.set import.Engine_ObjectEventFaceEachOther, 1
	.set import.Engine_ScriptSetMessage, 1
	.set import.Engine_ScriptShowMessage, 1
	.set import.Engine_ScriptRequestWarp, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
