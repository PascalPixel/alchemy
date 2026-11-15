.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.balign 4
	overlay_veneer Scene_Initialize
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Scene_GetActorPlacements
	overlay_veneer Scene_GetTriggers
	overlay_veneer Scene_GetExitRegions
	overlay_veneer Scene_PrepareMap
