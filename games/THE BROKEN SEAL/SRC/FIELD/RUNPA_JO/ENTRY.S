@ Entry veneers opening the RUNPA_JO overlay at its load address: fixed 8-byte
@ veneers through which the main image calls into the overlay, each loading
@ its target into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — RUNPA_JO overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_DispatchActorUpdate
	overlay_veneer SelectPrimarySceneData
	overlay_veneer SelectSecondarySceneData
	overlay_veneer SelectTertiarySceneData
	overlay_veneer SelectQuaternarySceneData
	overlay_veneer GetEmptySceneData
