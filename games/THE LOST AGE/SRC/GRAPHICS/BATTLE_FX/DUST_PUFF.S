@ BattleFx_DustPuffSheet: a 32x32 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_DustPuffSheet
BattleFx_DustPuffSheet:
	.incbin "GRAPHICS/BATTLE_FX/DUST_PUFF.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/DUST_PUFF.bitmap.lz"
