@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeStormyNight_Initialize
	overlay_veneer ValeStormyNight_GetWarpDests
	overlay_veneer ValeStormyNight_GetWarps
	overlay_veneer ValeStormyNight_GetObjectEventTemplates
	overlay_veneer ValeStormyNight_GetEvents
	overlay_veneer ValeStormyNight_GetRegions
