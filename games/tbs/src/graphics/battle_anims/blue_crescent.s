	.section .rodata
	.global BattleAnim_BlueCrescentSheet
BattleAnim_BlueCrescentSheet:
	.incbin "games/tbs/src/graphics/battle_anims/blue_crescent.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_crescent.bitmap.lz"
	.byte 0xe0, 0x02	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0x21e
