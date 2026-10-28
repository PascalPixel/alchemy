	.section .rodata
	.global BattleEffect_PinkBurstSheet
BattleEffect_PinkBurstSheet:
	.incbin "games/tla/src/graphics/battle_effects/pink_burst.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/pink_burst.bitmap.lz"
	.byte 0x57	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x36b
