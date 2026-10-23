@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_GameFlagClear, 1
	.set import.Engine_EventWait, 1
	.set import.Engine_EventStart, 1
	.set import.Engine_EventEnd, 1
	.set import.Object_GetById, 1
	.set import.Engine_ActorSetAnimationAndWait, 1
	.set import.Engine_ActorStartRepeatedMotion, 1
	.set import.Engine_ActorRunRepeatedMotion, 1
	.set import.Engine_ActorFaceActor, 1
	.set import.Engine_ActorFaceEachOther, 1
	.set import.Engine_EventSetMessage, 1
	.set import.Engine_EventShowMessage, 1
	.set import.Engine_EventRequestExit, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
