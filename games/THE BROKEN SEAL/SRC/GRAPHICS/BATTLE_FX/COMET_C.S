@ BattleFx_CometSheetC: a 32x48 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CometSheetC
BattleFx_CometSheetC:
	.incbin "GRAPHICS/BATTLE_FX/COMET_C.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/COMET_C.bitmap.lz"
