@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer TorebiMachi_ApplyEntryState
	overlay_veneer TorebiMachi_GetEntrances
	overlay_veneer TorebiMachi_GetExits
	overlay_veneer TorebiMachi_GetPlacements
	overlay_veneer TorebiMachi_SelectEvents
	overlay_veneer SceneData_ReturnZero
