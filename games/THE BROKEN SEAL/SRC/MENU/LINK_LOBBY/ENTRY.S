.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer LinkLobby_RunRoundResult
	overlay_veneer LinkLobby_GetEntrances
	overlay_veneer LinkLobby_GetExits
	overlay_veneer LinkLobby_SelectPlacements
	overlay_veneer LinkLobby_GetEvents
	overlay_veneer SceneData_ReturnZero
