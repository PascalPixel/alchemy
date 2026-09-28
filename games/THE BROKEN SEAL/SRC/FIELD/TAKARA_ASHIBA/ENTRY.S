@ Entry veneers opening the TAKARA_ASHIBA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — TAKARA_ASHIBA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02001f78
	overlay_veneer Func_020009f0
	overlay_veneer SceneData_GetTablea970
	overlay_veneer Func_02000a50
	overlay_veneer Func_02000ee0
	overlay_veneer SceneData_ReturnZero
