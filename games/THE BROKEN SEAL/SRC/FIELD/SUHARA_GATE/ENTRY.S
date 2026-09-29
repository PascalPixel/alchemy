@ Entry veneers opening the SUHARA_GATE overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SUHARA_GATE overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000124
	overlay_veneer Func_02000040
	overlay_veneer SceneData_GetSceneTable
	overlay_veneer Func_0200008c
	overlay_veneer Func_020000e4
	overlay_veneer SceneData_ReturnZero
