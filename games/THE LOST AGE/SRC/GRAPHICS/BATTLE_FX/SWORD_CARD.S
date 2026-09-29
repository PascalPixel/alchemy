@ BattleFx_SwordCardSheet: a 32x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SwordCardSheet
BattleFx_SwordCardSheet:
	.incbin "GRAPHICS/BATTLE_FX/SWORD_CARD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SWORD_CARD.bitmap.lz"
