@ BattleFx_CrystalSheet: a 48x104 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CrystalSheet
BattleFx_CrystalSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRYSTAL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRYSTAL.bitmap.lz"
