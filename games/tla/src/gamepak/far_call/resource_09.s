.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall009
Resource_FarCall009:
	.global Func_08118000
	.thumb_func
Func_08118000:
	overlay_veneer Func_08118958
	.global Func_08118008
	.thumb_func
Func_08118008:
	overlay_veneer Func_08125aa0
