@ BattleFx_MarsDjinnSheet: a 40x40 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_MarsDjinnSheet
BattleFx_MarsDjinnSheet:
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN.bitmap.lz"
