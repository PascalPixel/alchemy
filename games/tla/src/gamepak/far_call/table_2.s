.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global ResourceObject_CreateFar
	.thumb_func
ResourceObject_CreateFar:
	overlay_veneer ResourceObject_Create
	.global ResourceObject_ReleaseFar
	.thumb_func
ResourceObject_ReleaseFar:
	overlay_veneer ResourceObject_Release
	.global Func_08020050
	.thumb_func
Func_08020050:
	overlay_veneer Func_08022010
