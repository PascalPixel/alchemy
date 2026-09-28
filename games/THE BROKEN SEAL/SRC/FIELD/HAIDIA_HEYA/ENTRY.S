@ Entry veneers opening the HAIDIA_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — HAIDIA_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_RunSupplementalSequenceOne
	overlay_veneer SceneData_GetTable9478
	overlay_veneer SceneData_SelectTable9568ByFlag
	overlay_veneer SceneData_SelectFlaggedTable
	overlay_veneer SceneData_SelectTable9c00ByFlags
	overlay_veneer SceneData_ReturnZero
