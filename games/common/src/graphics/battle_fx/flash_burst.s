	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_FlashBurstSheet
BattleFx_FlashBurstSheet:
	.incbin "graphics/battle_fx/flash_burst.gbapal"
	.incbin "graphics/battle_fx/flash_burst.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x60	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x144b
	.endif
