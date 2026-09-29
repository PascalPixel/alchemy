@ BattleFx_VioletMoteSheet: a 24x12 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_VioletMoteSheet
BattleFx_VioletMoteSheet:
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_MOTE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_MOTE.bitmap.lz"
