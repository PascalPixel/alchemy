@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer Scene_InitFacingActors
	overlay_veneer TorebiHeya_GetEntrances
	overlay_veneer TorebiHeya_GetExits
	overlay_veneer TorebiHeya_SelectPlacements
	overlay_veneer TorebiHeya_SelectEvents
	overlay_veneer TorebiHeya_GetDefaultResult
