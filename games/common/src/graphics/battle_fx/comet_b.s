	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_CometSheetB
BattleFx_CometSheetB:
	.incbin "graphics/battle_fx/comet_b.gbapal"
	.incbin "graphics/battle_fx/comet_b.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0xaf	@ PACKER: left in the buffer by BattleFx_ThornSheet+0x2fb
	.endif
