@ Entry veneers opening the HAIDIA_ARASHI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — HAIDIA_ARASHI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_RunFlagGatedActorSetup
	overlay_veneer SceneData_GetTableD0E4
	overlay_veneer SceneData_GetTableD27c
	overlay_veneer SceneData_GetTableD2B8
	overlay_veneer SceneData_GetTableD558
	overlay_veneer SceneData_ReturnZero
