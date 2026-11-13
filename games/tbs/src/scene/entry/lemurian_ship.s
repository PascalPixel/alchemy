	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
	overlay_veneer LemurianShip_SetupMap
	overlay_veneer LemurianShip_GetEntrances
	overlay_veneer LemurianShip_GetExits
	overlay_veneer LemurianShip_GetActorPlacements
	overlay_veneer LemurianShip_GetTriggers
	overlay_veneer LemurianShip_GetRegions
