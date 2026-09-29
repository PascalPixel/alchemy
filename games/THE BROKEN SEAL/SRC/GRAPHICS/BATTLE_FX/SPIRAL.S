@ BattleFx_SpiralSheet: a 32x256 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SpiralSheet
BattleFx_SpiralSheet:
	.incbin "GRAPHICS/BATTLE_FX/SPIRAL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SPIRAL.bitmap.lz"
