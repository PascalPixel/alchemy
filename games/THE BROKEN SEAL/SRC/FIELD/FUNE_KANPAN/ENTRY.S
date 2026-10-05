@ Entry veneers opening the FUNE_KANPAN overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — FUNE_KANPAN overlay entry veneers
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
