@ BattleFx_GreatSwordSheet: a 32x128 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_GreatSwordSheet
BattleFx_GreatSwordSheet:
	.incbin "GRAPHICS/BATTLE_FX/GREAT_SWORD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GREAT_SWORD.bitmap.lz"
