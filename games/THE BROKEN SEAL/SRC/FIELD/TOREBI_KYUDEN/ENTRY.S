@ Entry veneers opening the TOREBI_KYUDEN overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — TOREBI_KYUDEN overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020040b4
	overlay_veneer Func_02000030
	overlay_veneer SceneData_GetTableca8c
	overlay_veneer Func_02000070
	overlay_veneer Func_02004034
	overlay_veneer SceneData_GetTableca7c
