	.section .rodata
	.global BattleAnim_FlashBurstSheet
BattleAnim_FlashBurstSheet:
	.incbin "games/tbs/src/graphics/battle_anims/flash_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/flash_burst.bitmap.lz"
	.byte 0x60	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0x144b
