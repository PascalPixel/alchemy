	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_CometSheetB
BattleFx_CometSheetB:
	.incbin "games/common/src/graphics/battle_fx/comet_b.gbapal"
	.incbin "games/common/src/graphics/battle_fx/comet_b.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0xaf	@ PACKER: left in the buffer by BattleFx_ThornSheet+0x2fb
	.else
	.ifdef TLA_EDITION_JA
	.byte 0xaf	@ PACKER: left in the buffer by BattleFx_ThornSheet+0x2fb
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
