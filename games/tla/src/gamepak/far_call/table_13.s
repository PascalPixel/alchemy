.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_080203a0
	.thumb_func
Func_080203a0:
	overlay_veneer Func_08026fc8
