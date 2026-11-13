	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
	overlay_veneer IncreaseItemAndLevel_Initialize
	overlay_veneer IncreaseItemAndLevel_GetEntrances
	overlay_veneer IncreaseItemAndLevel_GetExits
	overlay_veneer IncreaseItemAndLevel_GetActorPlacements
	overlay_veneer IncreaseItemAndLevel_GetTriggers
	overlay_veneer Scene_GetExitRegions
