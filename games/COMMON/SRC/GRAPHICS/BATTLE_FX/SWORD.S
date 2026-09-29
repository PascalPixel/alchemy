@ BattleFx_SwordSheet: a 72x60 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SwordSheet
BattleFx_SwordSheet:
	.incbin "GRAPHICS/BATTLE_FX/SWORD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SWORD.bitmap.lz"
