@ BattleFx_VenusDjinnSheet: a 40x40 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_VenusDjinnSheet
BattleFx_VenusDjinnSheet:
	.incbin "GRAPHICS/BATTLE_FX/VENUS_DJINN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VENUS_DJINN.bitmap.lz"
