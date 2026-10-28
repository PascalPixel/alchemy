	.section .rodata
	.global BattleEffect_BlueCrescentSheet
BattleEffect_BlueCrescentSheet:
	.incbin "games/tla/src/graphics/battle_effects/blue_crescent.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blue_crescent.bitmap.lz"
	.byte 0xe0, 0x02	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x21e
