@ BattleFx_ButterflyCardSheet: a 32x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_ButterflyCardSheet
BattleFx_ButterflyCardSheet:
	.incbin "GRAPHICS/BATTLE_FX/BUTTERFLY_CARD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BUTTERFLY_CARD.bitmap.lz"
