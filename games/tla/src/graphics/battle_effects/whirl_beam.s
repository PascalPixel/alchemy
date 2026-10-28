	.section .rodata
	.global BattleEffect_WhirlBeamSheet
BattleEffect_WhirlBeamSheet:
	.incbin "games/tla/src/graphics/battle_effects/whirl_beam.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/whirl_beam.bitmap.lz"
	.byte 0x54, 0xaa, 0x14	@ PACKER: left in the buffer by Summon_ChainedDemonTiles+0x2169
