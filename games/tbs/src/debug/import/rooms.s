@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_GameFlagClear, 1
	.set import.BattleFx_RunPageEffectForSlot, 1
	.include "games/tbs/src/system/overlay.inc"
