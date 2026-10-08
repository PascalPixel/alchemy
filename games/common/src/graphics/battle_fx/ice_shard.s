	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "graphics/battle_fx/ice_shard_00.gbapal"
	.incbin "graphics/battle_fx/ice_shard.parts.lz"
	.ifeq GAME_TLA
	.byte 0x3e	@ PACKER: left in the buffer by BattleFx_VioletStreakSheet+0xa33
	.endif
