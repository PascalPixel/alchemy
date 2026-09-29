@ BattleFx_BoulderSheet: a 64x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BoulderSheet
BattleFx_BoulderSheet:
	.incbin "GRAPHICS/BATTLE_FX/BOULDER.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BOULDER.bitmap.lz"
