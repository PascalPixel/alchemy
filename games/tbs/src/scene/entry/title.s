	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
	overlay_veneer Title_Run
	overlay_veneer Title_GetEntrances
	overlay_veneer Title_GetExits
	overlay_veneer Title_GetActorPlacements
	overlay_veneer Title_GetTriggers
	overlay_veneer Scene_GetExitRegions
