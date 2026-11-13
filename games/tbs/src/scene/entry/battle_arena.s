	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
	overlay_veneer BattleArena_Initialize
	overlay_veneer BattleArena_GetEntrances
	overlay_veneer BattleArena_GetExits
	overlay_veneer BattleArena_SelectActorPlacements
	overlay_veneer BattleArena_GetTriggers
	overlay_veneer Scene_GetExitRegions
