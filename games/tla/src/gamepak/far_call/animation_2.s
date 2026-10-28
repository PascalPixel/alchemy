.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Animation_SetIndexAndInitializeObjectsFar
	.thumb_func
Animation_SetIndexAndInitializeObjectsFar:
	overlay_veneer Animation_SetIndexAndInitializeObjects
	.global Func_08020240
	.thumb_func
Func_08020240:
	overlay_veneer Graphics_EnableObjLayerAndCallbacks
	.global Func_08020248
	.thumb_func
Func_08020248:
	overlay_veneer ActorSystem_ResumeProjectionAndHideLayers
	.global Animation_SetDisplayFlagFar
	.thumb_func
Animation_SetDisplayFlagFar:
	overlay_veneer Animation_SetDisplayFlag
