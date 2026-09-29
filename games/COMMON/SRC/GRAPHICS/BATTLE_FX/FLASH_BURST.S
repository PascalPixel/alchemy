@ BattleFx_FlashBurstSheet: a 32x384 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FlashBurstSheet
BattleFx_FlashBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLASH_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLASH_BURST.bitmap.lz"
