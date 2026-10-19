	.section .rodata
	.global BattleFx_FlashBurstSheet
BattleFx_FlashBurstSheet:
	.incbin "games/tla/src/graphics/battle_fx/flash_burst.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/flash_burst.bitmap.lz"
	.byte 0x60	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x144b
