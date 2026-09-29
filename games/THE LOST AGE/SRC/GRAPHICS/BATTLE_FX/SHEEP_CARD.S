@ BattleFx_SheepCardSheet: a 32x192 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SheepCardSheet
BattleFx_SheepCardSheet:
	.incbin "GRAPHICS/BATTLE_FX/SHEEP_CARD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SHEEP_CARD.bitmap.lz"
