@ BattleFx_ShurikenSheet: a 32x144 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_ShurikenSheet
BattleFx_ShurikenSheet:
	.incbin "GRAPHICS/BATTLE_FX/SHURIKEN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SHURIKEN.bitmap.lz"
