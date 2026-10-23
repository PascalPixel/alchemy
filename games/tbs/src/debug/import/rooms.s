@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.GameFlag_ClearBit, 1
	.set import.Djinn_JoinParty, 1
	.include "games/tbs/src/system/overlay.inc"
