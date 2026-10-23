@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer LinkLobby_RunRoundResult
	overlay_veneer LinkLobby_GetWarpDests
	overlay_veneer LinkLobby_GetWarps
	overlay_veneer LinkLobby_SelectObjectEventTemplates
	overlay_veneer LinkLobby_GetEvents
	overlay_veneer MapData_ReturnZero
