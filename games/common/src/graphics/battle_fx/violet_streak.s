	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_VioletStreakSheet
BattleFx_VioletStreakSheet:
	.incbin "games/common/src/graphics/battle_fx/violet_streak.gbapal"
	.incbin "games/common/src/graphics/battle_fx/violet_streak.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x5b	@ PACKER: left in the buffer by BattleFx_SpiralSheet+0xc8b
	.else
	.byte 0x5b	@ PACKER: left in the buffer by BattleFx_SpiralSheet+0xc8b
	.endif
