@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Scene_Initialize
	overlay_veneer RariberoHeya_GetEntrances
	overlay_veneer RariberoHeya_GetExits
	overlay_veneer RariberoHeya_GetPlacements
	overlay_veneer RariberoHeya_GetEvents
	overlay_veneer RariberoHeya_GetRegions
