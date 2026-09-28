@ Entry veneers opening the SORU_FUNKA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SORU_FUNKA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_RunWhenWord225Is10
	overlay_veneer SceneData_GetTableB6D4
	overlay_veneer SceneData_GetTableB704
	overlay_veneer SceneData_GetTableB710
	overlay_veneer SceneData_GetTableB998
	overlay_veneer Func_02000038
