@ Entry veneers opening the HAIDIA_BABI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — HAIDIA_BABI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer HaidiaBabi_RestoreEntryState
	overlay_veneer HaidiaBabi_GetEntrances
	overlay_veneer HaidiaBabi_SelectExits
	overlay_veneer HaidiaBabi_SelectPlacements
	overlay_veneer HaidiaBabi_SelectEvents
	overlay_veneer SceneData_ReturnZero
