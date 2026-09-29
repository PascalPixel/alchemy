@ BattleFx_FlameColumnSheet: a 16x128 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FlameColumnSheet
BattleFx_FlameColumnSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLAME_COLUMN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAME_COLUMN.bitmap.lz"
