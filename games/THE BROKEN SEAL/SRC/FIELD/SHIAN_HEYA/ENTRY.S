@ Entry veneers opening the SHIAN_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SHIAN_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer SceneState_SetRuntimeWord448To521
	overlay_veneer SceneData_GetTable8778
	overlay_veneer SceneData_GetTable8868
	overlay_veneer SceneData_SelectTable89c8Or8890
	overlay_veneer SceneData_SelectTable8d4cOr8a28
	overlay_veneer SceneData_ReturnZero
