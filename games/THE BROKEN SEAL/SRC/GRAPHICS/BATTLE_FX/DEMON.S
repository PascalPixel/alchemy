@ BattleFx_DemonFaceSheet: a 32x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_DemonFaceSheet
BattleFx_DemonFaceSheet:
	.incbin "GRAPHICS/BATTLE_FX/DEMON.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/DEMON.bitmap.lz"
