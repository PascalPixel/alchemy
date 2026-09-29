@ Entry veneers opening the KUUPUAPPU_DOU overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KUUPUAPPU_DOU overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer KuupuappuDou_ApplyEntryState
	overlay_veneer Func_02000030
	overlay_veneer Func_0200008c
	overlay_veneer Func_020000e0
	overlay_veneer Func_020002e0
	overlay_veneer SceneData_GetTable9C5C
