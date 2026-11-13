	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
	overlay_veneer ShipRooms_ApplyEntryState
	overlay_veneer ShipRooms_SelectEntrances
	overlay_veneer ShipRooms_GetExits
	overlay_veneer ShipRooms_SelectActorPlacements
	overlay_veneer ShipRooms_SelectTriggers
	overlay_veneer ShipRooms_GetRegions
