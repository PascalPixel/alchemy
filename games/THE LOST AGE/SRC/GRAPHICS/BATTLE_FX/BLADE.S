@ BattleFx_BladeSheet: a 40x80 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BladeSheet
BattleFx_BladeSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLADE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLADE.bitmap.lz"
