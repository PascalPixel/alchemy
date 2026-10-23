@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_WorkSetValuesIfNonNegative, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Resource37c_FinalizeSetup, 1
	.set import.Resource37c_QueueSoundCue, 1
	.include "games/tbs/src/system/overlay.inc"
