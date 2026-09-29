@ BattleFx_VineCardSheet: a 32x448 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_VineCardSheet
BattleFx_VineCardSheet:
	.incbin "GRAPHICS/BATTLE_FX/VINE_CARD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VINE_CARD.bitmap.lz"
