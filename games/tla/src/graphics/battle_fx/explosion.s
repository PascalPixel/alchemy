	.section .rodata
	.global BattleEffect_ExplosionSheet
BattleEffect_ExplosionSheet:
	.incbin "games/tla/src/graphics/battle_fx/explosion.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/explosion.bitmap.lz"
	.byte 0xec, 0x10, 0x3c	@ PACKER: left in the buffer by BattleEffect_WaterSpraySheet+0xdd5
