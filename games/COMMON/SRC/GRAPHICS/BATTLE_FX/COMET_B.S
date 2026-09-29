@ BattleFx_CometSheetB: a 32x36 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CometSheetB
BattleFx_CometSheetB:
	.incbin "GRAPHICS/BATTLE_FX/COMET_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/COMET_B.bitmap.lz"
