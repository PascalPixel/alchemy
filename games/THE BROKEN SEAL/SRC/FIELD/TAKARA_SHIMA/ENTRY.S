@ Entry veneers opening the TAKARA_SHIMA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — TAKARA_SHIMA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000f70
	overlay_veneer Func_02000cc4
	overlay_veneer GetDefaultSceneData
	overlay_veneer Func_02000d48
	overlay_veneer Func_02000ec4
	overlay_veneer GetEmptySceneData
