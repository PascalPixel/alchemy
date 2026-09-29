@ BattleFx_SheepSheet: a 32x128 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SheepSheet
BattleFx_SheepSheet:
	.incbin "GRAPHICS/BATTLE_FX/SHEEP.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SHEEP.bitmap.lz"
