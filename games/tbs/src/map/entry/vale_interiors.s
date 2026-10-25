@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeInteriors_Initialize
	overlay_veneer ValeInteriors_GetWarpDests
	overlay_veneer ValeInteriors_GetWarps
	overlay_veneer ValeInteriors_GetObjectEventTemplates
	overlay_veneer ValeInteriors_GetEvents
	overlay_veneer ValeInteriors_GetRegions
