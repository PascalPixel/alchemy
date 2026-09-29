@ BattleFx_AxeSheet: a 64x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_AxeSheet
BattleFx_AxeSheet:
	.incbin "GRAPHICS/BATTLE_FX/AXE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/AXE.bitmap.lz"
