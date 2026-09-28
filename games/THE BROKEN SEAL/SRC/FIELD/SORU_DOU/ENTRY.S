@ Entry veneers opening the SORU_DOU overlay at its load address: fixed 8-byte
@ veneers through which the main image calls into the overlay, each loading
@ its target into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — SORU_DOU overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Resource37c_Run
	overlay_veneer State_Run
	overlay_veneer SceneData_GetPrimaryTable37c
	overlay_veneer SceneData_GetSecondaryTable37c
	overlay_veneer SceneData_GetTertiaryTable37c
	overlay_veneer state_update_0432
