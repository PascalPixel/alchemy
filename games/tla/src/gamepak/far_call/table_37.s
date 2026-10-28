.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_080c8970
	.thumb_func
Func_080c8970:
	overlay_veneer BattleEffect_LoadBufferPalette
