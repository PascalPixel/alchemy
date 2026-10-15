@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Scene_InitFacingActors
	overlay_veneer TolbiInteriors_GetEntrances
	overlay_veneer TolbiInteriors_GetExits
	overlay_veneer TolbiInteriors_SelectPlacements
	overlay_veneer TolbiInteriors_SelectEvents
	overlay_veneer TolbiInteriors_GetDefaultResult
