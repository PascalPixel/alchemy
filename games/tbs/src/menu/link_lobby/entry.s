@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer LinkLobby_RunRoundResult
	overlay_veneer LinkLobby_GetEntrances
	overlay_veneer LinkLobby_GetExits
	overlay_veneer LinkLobby_SelectPlacements
	overlay_veneer LinkLobby_GetEvents
	overlay_veneer SceneData_ReturnZero
