	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
	overlay_veneer BabiPalace_ApplyEntryState
	overlay_veneer Scene_GetEntrances
	overlay_veneer BabiPalace_GetExits
	overlay_veneer Scene_GetActorPlacements
	overlay_veneer Scene_GetTriggers
	overlay_veneer BabiPalace_GetRegions
