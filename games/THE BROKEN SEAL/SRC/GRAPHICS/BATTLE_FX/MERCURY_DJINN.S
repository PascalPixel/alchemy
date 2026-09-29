@ BattleFx_MercuryDjinnSheet: a 40x40 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_MercuryDjinnSheet
BattleFx_MercuryDjinnSheet:
	.incbin "GRAPHICS/BATTLE_FX/MERCURY_DJINN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MERCURY_DJINN.bitmap.lz"
