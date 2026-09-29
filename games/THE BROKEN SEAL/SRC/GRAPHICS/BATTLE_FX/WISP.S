@ BattleFx_WispSheet: a 64x192 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_WispSheet
BattleFx_WispSheet:
	.incbin "GRAPHICS/BATTLE_FX/WISP.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WISP.bitmap.lz"
