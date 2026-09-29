@ BattleFx_CornerRuneBSheet: a 64x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CornerRuneBSheet
BattleFx_CornerRuneBSheet:
	.incbin "GRAPHICS/BATTLE_FX/CORNER_RUNE_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CORNER_RUNE_B.bitmap.lz"
