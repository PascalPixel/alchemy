.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020210
	.thumb_func
Func_08020210:
	overlay_veneer Func_0802d87c
	.global ObjectDispatch_SetSingleChildFlagsFar
	.thumb_func
ObjectDispatch_SetSingleChildFlagsFar:
	overlay_veneer ObjectDispatch_SetChildByteValue
	.global Animation_SetStateFlagsFar
	.thumb_func
Animation_SetStateFlagsFar:
	overlay_veneer Animation_SetStateFlags
