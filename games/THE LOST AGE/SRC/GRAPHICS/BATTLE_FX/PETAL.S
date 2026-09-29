@ BattleFx_PetalSheet: a 120x200 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_PetalSheet
BattleFx_PetalSheet:
	.incbin "GRAPHICS/BATTLE_FX/PETAL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PETAL.bitmap.lz"
