@ Entry veneers opening the GOMA_IKE overlay at its load address: fixed 8-byte
@ veneers through which the main image calls into the overlay, each loading
@ its target into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — GOMA_IKE overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_PlaceActor8OnGate300
	overlay_veneer SceneData_GetPrimaryTable
	overlay_veneer SceneData_GetSecondaryTable
	overlay_veneer SceneData_GetTertiaryTable
	overlay_veneer SceneData_GetQuaternaryTable
	overlay_veneer SceneData_ReturnZero
