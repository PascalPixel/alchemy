@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TolbiInteriors_ApplyEntryState
	overlay_veneer TolbiInteriors_GetWarpDests
	overlay_veneer TolbiInteriors_GetWarps
	overlay_veneer TolbiInteriors_SelectObjectEventTemplates
	overlay_veneer TolbiInteriors_SelectEvents
	overlay_veneer TolbiInteriors_GetRegions
