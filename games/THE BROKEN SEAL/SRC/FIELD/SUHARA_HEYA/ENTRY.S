.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_InitActorRecords
	overlay_veneer SuharaHeya_GetEntrances
	overlay_veneer SuharaHeya_GetExits
	overlay_veneer SuharaHeya_SelectPlacements
	overlay_veneer SuharaHeya_SelectEvents
	overlay_veneer SuharaHeya_GetRegions
