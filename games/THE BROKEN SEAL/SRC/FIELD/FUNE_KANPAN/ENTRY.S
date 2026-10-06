.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FuneKanpan_ResetDeck
	overlay_veneer FuneKanpan_GetEntrances
	overlay_veneer FuneKanpan_GetExits
	overlay_veneer FuneKanpan_GetPlacements
	overlay_veneer FuneKanpan_GetEvents
	overlay_veneer FuneKanpan_GetRegions
