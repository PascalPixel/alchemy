@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.MapGrid_SetScreenShake, 1
	.set import.GameFlag_Get, 1
	.set import.Engine_GameFlagSet, 1
	.set import.FieldEffect_InitializeSparkles, 1
	.set import.Map_QueueMusic, 1
	.include "games/tbs/src/system/overlay.inc"
