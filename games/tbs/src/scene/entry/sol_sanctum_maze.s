	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
	overlay_veneer SolSanctumRooms_Initialize
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Scene_GetActorPlacements
	overlay_veneer Scene_GetTriggers
	overlay_veneer Scene_GetExitRegions
