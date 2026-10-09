@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer CommandTable_ConfigureCommandList
	overlay_veneer MenuTest_GetEntrances
	overlay_veneer MenuTest_GetExits
	overlay_veneer MenuTest_GetPlacements
	overlay_veneer MenuTest_GetEvents
	overlay_veneer SceneData_ReturnZero
