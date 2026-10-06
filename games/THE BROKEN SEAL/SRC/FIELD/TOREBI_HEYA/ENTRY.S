.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_InitFacingActors
	overlay_veneer TorebiHeya_GetEntrances
	overlay_veneer TorebiHeya_GetExits
	overlay_veneer TorebiHeya_SelectPlacements
	overlay_veneer TorebiHeya_SelectEvents
	overlay_veneer TorebiHeya_GetDefaultResult
