	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_FlashBurstSheet
BattleFx_FlashBurstSheet:
	.incbin "games/common/src/graphics/battle_fx/flash_burst.gbapal"
	.incbin "games/common/src/graphics/battle_fx/flash_burst.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x60	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x144b
	.else
	.byte 0x60	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x144b
	.endif
