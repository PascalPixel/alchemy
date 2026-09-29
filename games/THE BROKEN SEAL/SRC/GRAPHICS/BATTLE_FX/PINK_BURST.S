@ BattleFx_PinkBurstSheet: a 24x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_PinkBurstSheet
BattleFx_PinkBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/PINK_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PINK_BURST.bitmap.lz"
