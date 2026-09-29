@ Entry veneers opening the KAREI_KYUDEN overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KAREI_KYUDEN overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000230
	overlay_veneer Func_02000030
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer Func_0200006c
	overlay_veneer Func_020000a4
	overlay_veneer SceneData_ReturnZero
