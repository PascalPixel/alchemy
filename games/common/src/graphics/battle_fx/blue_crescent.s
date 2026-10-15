	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_BlueCrescentSheet
BattleFx_BlueCrescentSheet:
	.incbin "games/common/src/graphics/battle_fx/blue_crescent.gbapal"
	.incbin "games/common/src/graphics/battle_fx/blue_crescent.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0xe0, 0x02	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x21e
	.else
	.byte 0xe0, 0x02	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x21e
	.endif
