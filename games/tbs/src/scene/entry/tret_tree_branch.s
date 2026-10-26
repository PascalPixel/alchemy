@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TretTreeBranch_Initialize
	overlay_veneer TretTreeBranch_GetWarpDests
	overlay_veneer TretTreeBranch_GetWarps
	overlay_veneer TretTreeBranch_GetObjectEventTemplates
	overlay_veneer TretTreeBranch_GetEvents
	overlay_veneer TretTreeBranch_GetRegions
