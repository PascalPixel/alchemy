@ BattleFx_LimeColumnSheet: a 56x20 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_LimeColumnSheet
BattleFx_LimeColumnSheet:
	.incbin "GRAPHICS/BATTLE_FX/LIME_COLUMN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/LIME_COLUMN.bitmap.lz"
