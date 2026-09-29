@ Entry veneers opening the FUNE_KANPAN overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — FUNE_KANPAN overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020013d4
	overlay_veneer SceneData_GetTablec994
	overlay_veneer SceneData_GetTablecb64
	overlay_veneer FuneKanpan_GetPlacements
	overlay_veneer FuneKanpan_GetEvents
	overlay_veneer SceneData_GetTablecb44
