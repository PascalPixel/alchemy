@ BattleFx_BlueArcSheetA: a 48x432 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BlueArcSheetA
BattleFx_BlueArcSheetA:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_A.bitmap.lz"
