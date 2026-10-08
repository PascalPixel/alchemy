	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_PinkBurstSheet
BattleFx_PinkBurstSheet:
	.incbin "graphics/battle_fx/pink_burst.gbapal"
	.incbin "graphics/battle_fx/pink_burst.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x57	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x36b
	.endif
