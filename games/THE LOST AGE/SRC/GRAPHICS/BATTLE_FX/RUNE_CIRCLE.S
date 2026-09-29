@ BattleFx_RuneCircleSheet: a 128x128 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RuneCircleSheet
BattleFx_RuneCircleSheet:
	.incbin "GRAPHICS/BATTLE_FX/RUNE_CIRCLE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RUNE_CIRCLE.bitmap.lz"
