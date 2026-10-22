	.section .rodata
	.global BattleEffect_FireballSheet
BattleEffect_FireballSheet:
	.incbin "games/tla/src/graphics/battle_fx/fireball.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/fireball.bitmap.lz"
	.byte 0x32	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x5df
