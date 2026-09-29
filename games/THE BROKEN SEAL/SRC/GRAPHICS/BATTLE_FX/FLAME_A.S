@ BattleFx_FlameSheetA: a 32x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FlameSheetA
BattleFx_FlameSheetA:
	.incbin "GRAPHICS/BATTLE_FX/FLAME_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAME_A.bitmap.lz"
