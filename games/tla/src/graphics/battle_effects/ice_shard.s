	.section .rodata
	.global BattleEffect_IceShardSheet
BattleEffect_IceShardSheet:
	.incbin "games/tla/src/graphics/battle_effects/ice_shard_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/ice_shard.parts.lz"
	.byte 0x3e	@ PACKER: left in the buffer by BattleEffect_VioletStreakSheet+0xa33
