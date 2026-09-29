@ BattleFx_BlueArcSheetB: a 48x432 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BlueArcSheetB
BattleFx_BlueArcSheetB:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_B.bitmap.lz"
