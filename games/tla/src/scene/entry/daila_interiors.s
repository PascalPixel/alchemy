.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.balign 4
	overlay_veneer DailaInteriors_Start
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Scene_GetPlacements
	overlay_veneer Scene_GetEvents
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
