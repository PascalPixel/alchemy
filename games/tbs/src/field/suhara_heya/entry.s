.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer Scene_InitActorRecords
	overlay_veneer SuharaHeya_GetEntrances
	overlay_veneer SuharaHeya_GetExits
	overlay_veneer SuharaHeya_SelectPlacements
	overlay_veneer SuharaHeya_SelectEvents
	overlay_veneer SuharaHeya_GetRegions
