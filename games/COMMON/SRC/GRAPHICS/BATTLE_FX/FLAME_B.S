@ BattleFx_FlameSheetB: a 32x252 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FlameSheetB
BattleFx_FlameSheetB:
	.incbin "GRAPHICS/BATTLE_FX/FLAME_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAME_B.bitmap.lz"
