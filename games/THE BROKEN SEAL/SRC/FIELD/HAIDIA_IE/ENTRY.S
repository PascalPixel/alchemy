@ Entry veneers opening the HAIDIA_IE overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — HAIDIA_IE overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer HaidiaIe_RestoreEntryState
	overlay_veneer SceneData_GetTableAfa0
	overlay_veneer SceneData_SelectTableByFlag834
	overlay_veneer SceneData_SelectTableByFlags834And87a
	overlay_veneer SceneData_SelectTableByFlags87a_815_834
	overlay_veneer Func_02000090
