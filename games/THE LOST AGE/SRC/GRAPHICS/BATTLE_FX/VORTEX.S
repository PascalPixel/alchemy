@ BattleFx_VortexSheet: a 16x210 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_VortexSheet
BattleFx_VortexSheet:
	.incbin "GRAPHICS/BATTLE_FX/VORTEX.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VORTEX.bitmap.lz"
