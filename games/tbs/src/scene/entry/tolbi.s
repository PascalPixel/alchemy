	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
	overlay_veneer Tolbi_ApplyEntryState
	overlay_veneer Tolbi_GetEntrances
	overlay_veneer Tolbi_GetExits
	overlay_veneer Tolbi_GetActorPlacements
	overlay_veneer Tolbi_SelectTriggers
	overlay_veneer Tolbi_GetRegions
