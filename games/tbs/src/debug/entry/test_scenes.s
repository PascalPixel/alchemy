	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
	overlay_veneer Test_Initialize
	overlay_veneer Test_GetEntrances
	overlay_veneer Test_GetExits
	overlay_veneer Test_GetActorPlacements
	overlay_veneer Test_GetTriggers
	overlay_veneer Scene_GetExitRegions
