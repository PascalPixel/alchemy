@ Entry veneers opening the TOREBI_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — TOREBI_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_InitFacingActors
	overlay_veneer SceneData_GetTable8BB4
	overlay_veneer SceneData_GetTable8dac
	overlay_veneer SceneData_SelectTable8e00ByFlag
	overlay_veneer SceneData_SelectTable9310ByFlags
	overlay_veneer Func_02000038
