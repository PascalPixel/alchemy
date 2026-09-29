@ Entry veneers opening the SUHARA_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SUHARA_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_InitActorRecords
	overlay_veneer SuharaHeya_GetEntrances
	overlay_veneer SuharaHeya_GetExits
	overlay_veneer SuharaHeya_SelectPlacements
	overlay_veneer SuharaHeya_SelectEvents
	overlay_veneer SuharaHeya_GetRegions
