.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_RunSupplementalSequenceOne
	overlay_veneer HaidiaHeya_GetEntrances
	overlay_veneer HaidiaHeya_SelectExits
	overlay_veneer SceneData_SelectFlaggedTable
	overlay_veneer HaidiaHeya_SelectEvents
	overlay_veneer SceneData_ReturnZero
