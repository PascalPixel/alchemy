.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Runtime_SetWorkTripleIfNonNegativeFar
	.thumb_func
Runtime_SetWorkTripleIfNonNegativeFar:
	overlay_veneer Runtime_SetWorkTripleIfNonNegative
	.global Func_08020230
	.thumb_func
Func_08020230:
	overlay_veneer Func_0802dcd8
