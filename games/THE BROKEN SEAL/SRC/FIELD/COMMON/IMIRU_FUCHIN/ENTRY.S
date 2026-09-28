@ Entry veneers opening the IMIRU_FUCHIN overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — IMIRU_FUCHIN overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02001750
	overlay_veneer Func_02000050
	overlay_veneer ImiruFuchin_GetExits
	overlay_veneer Func_020000f8
	overlay_veneer Func_02000f30
	overlay_veneer SceneData_ReturnZero
