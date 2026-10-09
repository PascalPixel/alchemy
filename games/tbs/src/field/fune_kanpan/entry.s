@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer FuneKanpan_ResetDeck
	overlay_veneer FuneKanpan_GetEntrances
	overlay_veneer FuneKanpan_GetExits
	overlay_veneer FuneKanpan_GetPlacements
	overlay_veneer FuneKanpan_GetEvents
	overlay_veneer FuneKanpan_GetRegions
