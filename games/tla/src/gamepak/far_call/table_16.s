.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_08038200
	.thumb_func
Func_08038200:
	overlay_veneer UiTimedNotice_Create
	.global UiTimedNotice_CloseIfActiveFar
	.thumb_func
UiTimedNotice_CloseIfActiveFar:
	overlay_veneer UiTimedNotice_CloseIfActive
	.global Func_08038210
	.thumb_func
Func_08038210:
	overlay_veneer Func_0803ce64
