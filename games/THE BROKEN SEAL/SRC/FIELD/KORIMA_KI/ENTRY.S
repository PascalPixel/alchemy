@ Entry veneers opening the KORIMA_KI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KORIMA_KI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02001070
	overlay_veneer KorimaKi_GetEntrances
	overlay_veneer KorimaKi_GetExits
	overlay_veneer KorimaKi_GetPlacements
	overlay_veneer KorimaKi_GetEvents
	overlay_veneer KorimaKi_GetRegions
