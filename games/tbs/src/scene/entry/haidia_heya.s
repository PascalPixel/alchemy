@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Scene_RunSupplementalSequenceOne
	overlay_veneer HaidiaHeya_GetEntrances
	overlay_veneer HaidiaHeya_SelectExits
	overlay_veneer SceneData_SelectFlaggedTable
	overlay_veneer HaidiaHeya_SelectEvents
	overlay_veneer SceneData_ReturnZero
