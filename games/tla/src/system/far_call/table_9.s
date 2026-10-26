.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_08020340
	.thumb_func
Func_08020340:
	overlay_veneer Func_080237e8
	.global Runtime_CheckRadiusOverlapFar
	.thumb_func
Runtime_CheckRadiusOverlapFar:
	overlay_veneer Runtime_CheckRadiusOverlap
	.global Func_08020350
	.thumb_func
Func_08020350:
	overlay_veneer Func_0802da88
