@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KalayInteriors_Initialize
	overlay_veneer KalayInteriors_GetWarpDests
	overlay_veneer KalayInteriors_GetWarps
	overlay_veneer KalayInteriors_GetObjectEventTemplates
	overlay_veneer KalayInteriors_GetEvents
	overlay_veneer KalayInteriors_GetRegions
