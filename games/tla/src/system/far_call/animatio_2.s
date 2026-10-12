.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Animation_SetIndexAndInitObjectsFar
	.thumb_func
Animation_SetIndexAndInitObjectsFar:
	overlay_veneer Animation_SetIndexAndInitObjects
	.global Func_08020240
	.thumb_func
Func_08020240:
	overlay_veneer Graphics_EnableObjLayerAndCallbacks
	.global Func_08020248
	.thumb_func
Func_08020248:
	overlay_veneer ObjectDispatch_StopCallbacksAndHideLayers
	.global Animation_SetDisplayFlagFar
	.thumb_func
Animation_SetDisplayFlagFar:
	overlay_veneer Animation_SetDisplayFlag
