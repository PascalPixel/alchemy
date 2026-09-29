@ Entry veneers opening the IMIRU_MURA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — IMIRU_MURA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000b70
	overlay_veneer Func_02000130
	overlay_veneer SceneData_GetTableA990
	overlay_veneer Func_0200016c
	overlay_veneer Func_020001ec
	overlay_veneer SceneData_ReturnZero
