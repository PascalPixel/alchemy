@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TolbiSpring_Initialize
	overlay_veneer TolbiSpring_GetWarpDests
	overlay_veneer TolbiSpring_GetWarps
	overlay_veneer TolbiSpring_GetObjectEventTemplates
	overlay_veneer TolbiSpring_GetEvents
	overlay_veneer TolbiSpring_GetRegions
