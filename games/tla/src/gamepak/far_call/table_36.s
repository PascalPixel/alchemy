.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_080c8950
	.thumb_func
Func_080c8950:
	overlay_veneer Func_080d072c
	.global Func_080c8958
	.thumb_func
Func_080c8958:
	overlay_veneer Func_080d25c8
	.global SceneEvent_FindActiveRecordFar
	.thumb_func
SceneEvent_FindActiveRecordFar:
	overlay_veneer Func_080ccd78
	.global Func_080c8968
	.thumb_func
Func_080c8968:
	overlay_veneer Func_080d1760
