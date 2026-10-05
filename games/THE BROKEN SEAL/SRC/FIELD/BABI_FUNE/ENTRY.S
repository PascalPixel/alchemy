@ Entry veneers opening the BABI_FUNE overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — BABI_FUNE overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer BabiFune_SetupScene
	overlay_veneer BabiFune_GetEntrances
	overlay_veneer BabiFune_GetExits
	overlay_veneer BabiFune_GetPlacements
	overlay_veneer BabiFune_GetEvents
	overlay_veneer BabiFune_GetDefaultResult
