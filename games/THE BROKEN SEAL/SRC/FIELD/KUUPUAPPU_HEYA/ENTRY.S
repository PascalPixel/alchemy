@ Entry veneers opening the KUUPUAPPU_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KUUPUAPPU_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer KuupuappuHeya_ApplyEntryState
	overlay_veneer SceneData_GetTableD8f8
	overlay_veneer SceneData_GetTableDa80
	overlay_veneer Func_02000048
	overlay_veneer Func_02000310
	overlay_veneer SceneData_GetTableDa60
