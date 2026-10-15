	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_DustPuffSheet
BattleFx_DustPuffSheet:
	.incbin "games/common/src/graphics/battle_fx/dust_puff.gbapal"
	.incbin "games/common/src/graphics/battle_fx/dust_puff.bitmap.lz"
	.ifeq GAME_TLA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.byte 0x7e, 0xe0, 0x98	@ PACKER: left in the buffer by BattleFx_LightFanSheet+0x339
	.endif
