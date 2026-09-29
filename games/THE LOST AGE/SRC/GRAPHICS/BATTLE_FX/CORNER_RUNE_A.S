@ BattleFx_CornerRuneASheet: a 64x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CornerRuneASheet
BattleFx_CornerRuneASheet:
	.incbin "GRAPHICS/BATTLE_FX/CORNER_RUNE_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CORNER_RUNE_A.bitmap.lz"
