@ BattleFx_DragonCardSheet: a 32x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_DragonCardSheet
BattleFx_DragonCardSheet:
	.incbin "GRAPHICS/BATTLE_FX/DRAGON_CARD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/DRAGON_CARD.bitmap.lz"
