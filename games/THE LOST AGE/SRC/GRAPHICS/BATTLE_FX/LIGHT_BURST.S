@ BattleFx_LightBurstSheet: a 64x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_LightBurstSheet
BattleFx_LightBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/LIGHT_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/LIGHT_BURST.bitmap.lz"
