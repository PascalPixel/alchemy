	.section .rodata
	.global BattleAnim_ExplosionSheet
BattleAnim_ExplosionSheet:
	.incbin "games/tbs/src/graphics/battle_fx/explosion.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/explosion.bitmap.lz"
	.byte 0xec, 0x10, 0x3c	@ PACKER: left in the buffer by BattleAnim_WaterSpraySheet+0xdd5
