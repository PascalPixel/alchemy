@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer MogallForest_Initialize
	overlay_veneer MogallForest_GetWarpDests
	overlay_veneer MogallForest_GetWarps
	overlay_veneer MogallForest_GetObjectEventTemplates
	overlay_veneer MogallForest_GetEvents
	overlay_veneer MogallForest_GetRegions
