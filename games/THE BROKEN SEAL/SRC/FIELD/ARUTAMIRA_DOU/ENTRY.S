@ Entry veneers opening the ARUTAMIRA_DOU overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — ARUTAMIRA_DOU overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02003644
	overlay_veneer SceneData_GetTablebf70
	overlay_veneer SceneData_GetTablec138
	overlay_veneer Func_02000a00
	overlay_veneer Func_02003598
	overlay_veneer SceneData_ReturnZero
