@ BattleFx_BlueBurstSheet: a 40x32 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BlueBurstSheet
BattleFx_BlueBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BURST.bitmap.lz"
