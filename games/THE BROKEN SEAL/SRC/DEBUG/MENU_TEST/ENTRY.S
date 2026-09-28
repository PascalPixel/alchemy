@ Entry veneers opening the MENU_TEST overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — MENU_TEST overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer CommandTable_ConfigureCommandList
	overlay_veneer SceneData_GetTable93c8
	overlay_veneer SceneData_GetTable93f8
	overlay_veneer SceneData_GetTable93fc
	overlay_veneer SceneData_GetTable9564
	overlay_veneer SceneData_ReturnZero
