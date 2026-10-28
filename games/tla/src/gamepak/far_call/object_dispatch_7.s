.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_08020258
	.thumb_func
Func_08020258:
	overlay_veneer Func_0802da24
	.global Actor_RegisterSpriteMetadataFar
	.thumb_func
Actor_RegisterSpriteMetadataFar:
	overlay_veneer Actor_RegisterSpriteMetadata
