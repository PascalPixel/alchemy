.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_RunEmptyScene
	overlay_veneer TestRooms_GetEntrances
	overlay_veneer TestRooms_GetExits
	overlay_veneer TestRooms_GetPlacements
	overlay_veneer TestRooms_GetEvents
	overlay_veneer SceneData_ReturnZero
