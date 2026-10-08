	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "GRAPHICS/BATTLE_FX/ICE_SHARD_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ICE_SHARD.parts.lz"
	.ifeq GAME_TLA
	.byte 0x3e	@ PACKER: left in the buffer by BattleFx_VioletStreakSheet+0xa33
	.endif
