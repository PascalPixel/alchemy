@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_WorkSetValuesIfNonNegative, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.InitializeSparkleRecordBuffer, 1
	.set import.QueueMapSound, 1
	.include "games/tbs/src/system/overlay.inc"
