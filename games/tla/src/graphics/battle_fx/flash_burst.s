	.section .rodata
	.global BattleEffect_FlashBurstSheet
BattleEffect_FlashBurstSheet:
	.incbin "games/tla/src/graphics/battle_fx/flash_burst.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/flash_burst.bitmap.lz"
	.byte 0x60	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x144b
