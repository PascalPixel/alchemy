@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer XianInteriors_Initialize
	overlay_veneer XianInteriors_GetWarpDests
	overlay_veneer XianInteriors_GetWarps
	overlay_veneer XianInteriors_GetObjectEventTemplates
	overlay_veneer XianInteriors_GetEvents
	overlay_veneer XianInteriors_GetRegions
