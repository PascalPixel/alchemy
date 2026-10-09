@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
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
