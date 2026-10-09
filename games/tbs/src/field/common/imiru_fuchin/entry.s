@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer ImiruFuchin_ApplyEntryHook
	overlay_veneer Scene_GetEntrances
	overlay_veneer ImiruFuchin_GetExits
	overlay_veneer Scene_GetPlacements
	overlay_veneer Scene_GetEvents
	overlay_veneer SceneData_ReturnZero
