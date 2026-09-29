@ Entry veneers opening the SHIAN_MURA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SHIAN_MURA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ShianMura_SetupScene
	overlay_veneer SceneEffect_GetPrimaryData
	overlay_veneer SceneEffect_GetSecondaryData
	overlay_veneer SceneEffect_PrepareState
	overlay_veneer SceneEffect_GetTertiaryData
	overlay_veneer SceneEffect_GetInitialValue
