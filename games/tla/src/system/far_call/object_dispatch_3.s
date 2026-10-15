.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080200c0
	.thumb_func
Func_080200c0:
	overlay_veneer Func_08023220
	.global Object_Destroy
	.thumb_func
Object_Destroy:
	overlay_veneer ObjectDispatch_Release
	.global ObjectDispatch_SetChildSignedValueFar
	.thumb_func
ObjectDispatch_SetChildSignedValueFar:
	overlay_veneer ObjectDispatch_SetChildSignedValue
