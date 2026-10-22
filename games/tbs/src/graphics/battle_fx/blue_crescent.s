	.section .rodata
	.global BattleEffect_BlueCrescentSheet
BattleEffect_BlueCrescentSheet:
	.incbin "games/tbs/src/graphics/battle_fx/blue_crescent.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/blue_crescent.bitmap.lz"
	.byte 0xe0, 0x02	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x21e
