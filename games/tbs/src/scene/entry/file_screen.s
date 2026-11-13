	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
	overlay_veneer FileScreen_Run
	overlay_veneer FileScreen_GetEntrances
	overlay_veneer FileScreen_GetExits
	overlay_veneer FileScreen_GetActorPlacements
	overlay_veneer FileScreen_GetTriggers
	overlay_veneer Scene_GetExitRegions
