@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer BattleArena_RunRoundResult
	overlay_veneer BattleArena_GetWarpDests
	overlay_veneer BattleArena_GetWarps
	overlay_veneer BattleArena_SelectObjectEventTemplates
	overlay_veneer BattleArena_GetEvents
	overlay_veneer MapData_ReturnZero
