	.section .rodata
	.global BattleEffect_FireballSheet
BattleEffect_FireballSheet:
	.incbin "games/tla/src/graphics/battle_effects/fireball.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/fireball.bitmap.lz"
	.byte 0x32	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x5df
