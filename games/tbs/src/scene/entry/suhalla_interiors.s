@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Scene_InitActorRecords
	overlay_veneer SuhallaInteriors_GetEntrances
	overlay_veneer SuhallaInteriors_GetExits
	overlay_veneer SuhallaInteriors_SelectPlacements
	overlay_veneer SuhallaInteriors_SelectEvents
	overlay_veneer SuhallaInteriors_GetRegions
