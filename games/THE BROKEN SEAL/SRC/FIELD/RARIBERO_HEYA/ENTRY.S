.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_Initialize
	overlay_veneer RariberoHeya_GetEntrances
	overlay_veneer RariberoHeya_GetExits
	overlay_veneer RariberoHeya_GetPlacements
	overlay_veneer RariberoHeya_GetEvents
	overlay_veneer RariberoHeya_GetRegions
