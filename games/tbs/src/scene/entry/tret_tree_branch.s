@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TretTreeBranch_InitializeBackdropTransition
	overlay_veneer TretTreeBranch_GetEntrances
	overlay_veneer TretTreeBranch_GetExits
	overlay_veneer TretTreeBranch_GetPlacements
	overlay_veneer TretTreeBranch_GetEvents
	overlay_veneer SceneData_ReturnZero
