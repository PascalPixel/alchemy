@ Entry veneers opening the TORETO_EDA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — TORETO_EDA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer State_SetRuntimeWord448To256
	overlay_veneer SceneData_GetTable835c
	overlay_veneer SceneData_GetTable844c
	overlay_veneer SceneData_GetTable8474
	overlay_veneer SceneData_GetTable84a4
	overlay_veneer SceneData_ReturnZero
