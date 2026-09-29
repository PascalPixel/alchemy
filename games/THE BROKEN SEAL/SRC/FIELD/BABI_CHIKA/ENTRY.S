@ Entry veneers opening the BABI_CHIKA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — BABI_CHIKA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_InitializeActorGroups
	overlay_veneer Scene_GetEntrances
	overlay_veneer SceneData_GetTableB85c
	overlay_veneer SceneData_SelectAndApplyTableBySceneId
	overlay_veneer SceneData_SelectTableByWord224
	overlay_veneer Scene_GetRegions
