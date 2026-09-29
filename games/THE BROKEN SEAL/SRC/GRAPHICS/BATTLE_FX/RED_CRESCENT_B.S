@ BattleFx_RedCrescentSheetB: a 128x205 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RedCrescentSheetB
BattleFx_RedCrescentSheetB:
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_B.bitmap.lz"
