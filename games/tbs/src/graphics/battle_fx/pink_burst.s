	.section .rodata
	.global BattleAnim_PinkBurstSheet
BattleAnim_PinkBurstSheet:
	.incbin "games/tbs/src/graphics/battle_fx/pink_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/pink_burst.bitmap.lz"
	.byte 0x57	@ PACKER: left in the buffer by BattleAnim_IceBlockSheet+0x36b
