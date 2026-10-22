	.section .rodata
	.global BattleEffect_FirePillarSheetB
BattleEffect_FirePillarSheetB:
	.incbin "games/tla/src/graphics/battle_fx/fire_pillar_b.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/fire_pillar_b.bitmap.lz"
	.byte 0x0d, 0x03	@ PACKER: left in the buffer by BattleEffect_BlastSheet+0x11fe
