@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer BabiLighthouse_SetupMap
	overlay_veneer Scene_GetWarpDests
	overlay_veneer BabiLighthouse_GetWarps
	overlay_veneer Scene_GetObjectEventTemplates
	overlay_veneer Scene_GetEvents
	overlay_veneer Scene_GetWarpRegions
