@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Map_Initialize
	overlay_veneer LaliveroInteriors_GetWarpDests
	overlay_veneer LaliveroInteriors_GetWarps
	overlay_veneer LaliveroInteriors_GetObjectEventTemplates
	overlay_veneer LaliveroInteriors_GetEvents
	overlay_veneer LaliveroInteriors_GetRegions
