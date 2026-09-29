@ BattleFx_SkullSheet: a 24x72 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SkullSheet
BattleFx_SkullSheet:
	.incbin "GRAPHICS/BATTLE_FX/SKULL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SKULL.bitmap.lz"
