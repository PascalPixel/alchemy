@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TretBranch_InitializeBackdropTransition
	overlay_veneer TretBranch_GetEntrances
	overlay_veneer TretBranch_GetExits
	overlay_veneer TretBranch_GetPlacements
	overlay_veneer TretBranch_GetEvents
	overlay_veneer SceneData_ReturnZero
