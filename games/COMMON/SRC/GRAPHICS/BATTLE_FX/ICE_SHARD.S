	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "GRAPHICS/BATTLE_FX/ICE_SHARD_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ICE_SHARD.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_VioletStreakSheet at +0xa33 */
	.byte 0x3e
	.endif
