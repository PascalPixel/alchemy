	.include "games/tla/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_CometSheetB
BattleFx_CometSheetB:
	.incbin "games/tla/src/graphics/battle_fx/comet_b.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/comet_b.bitmap.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xaf	@ PACKER: left in the buffer by BattleFx_ThornSheet+0x2fb
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
