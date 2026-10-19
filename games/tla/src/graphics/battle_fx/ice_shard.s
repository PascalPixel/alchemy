	.section .rodata
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "games/tla/src/graphics/battle_fx/ice_shard_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/ice_shard.parts.lz"
	.byte 0x3e	@ PACKER: left in the buffer by BattleFx_VioletStreakSheet+0xa33
