@ BattleFx_RuneSheet: a 24x48 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RuneSheet
BattleFx_RuneSheet:
	.incbin "GRAPHICS/BATTLE_FX/RUNE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RUNE.bitmap.lz"
