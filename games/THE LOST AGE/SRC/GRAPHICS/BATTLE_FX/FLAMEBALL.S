@ BattleFx_FlameballSheet: a 40x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FlameballSheet
BattleFx_FlameballSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLAMEBALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAMEBALL.bitmap.lz"
