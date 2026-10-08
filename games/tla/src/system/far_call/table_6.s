.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Runtime_SetWorkTripleIfNonNegativeFar
	.thumb_func
Runtime_SetWorkTripleIfNonNegativeFar:
	overlay_veneer Runtime_SetWorkTripleIfNonNegative
	.global Func_08020230
	.thumb_func
Func_08020230:
	overlay_veneer Map_WaitWorkValuesBelow256
