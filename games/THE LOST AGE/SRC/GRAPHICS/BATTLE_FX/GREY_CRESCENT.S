@ BattleFx_GreyCrescentSheet: a 128x256 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_GreyCrescentSheet
BattleFx_GreyCrescentSheet:
	.incbin "GRAPHICS/BATTLE_FX/GREY_CRESCENT.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GREY_CRESCENT.bitmap.lz"
