@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Scene_RunEmptyScene
	overlay_veneer TestRooms_GetEntrances
	overlay_veneer TestRooms_GetExits
	overlay_veneer TestRooms_GetPlacements
	overlay_veneer TestRooms_GetEvents
	overlay_veneer SceneData_ReturnZero
