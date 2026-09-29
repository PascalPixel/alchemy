@ BattleFx_CometSheetA: a 32x24 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CometSheetA
BattleFx_CometSheetA:
	.incbin "GRAPHICS/BATTLE_FX/COMET_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/COMET_A.bitmap.lz"
