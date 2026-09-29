@ BattleFx_CornerGlowSheet: a 64x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CornerGlowSheet
BattleFx_CornerGlowSheet:
	.incbin "GRAPHICS/BATTLE_FX/CORNER_GLOW.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CORNER_GLOW.bitmap.lz"
