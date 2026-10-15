	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "games/common/src/graphics/battle_fx/ice_shard_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/ice_shard.parts.lz"
	.ifne GAME_TLA
	.byte 0x3e	@ PACKER: left in the buffer by BattleFx_VioletStreakSheet+0xa33
	.endif
