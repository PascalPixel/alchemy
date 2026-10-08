	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_CometSheetA
BattleFx_CometSheetA:
	.incbin "graphics/battle_fx/comet_a.gbapal"
	.incbin "graphics/battle_fx/comet_a.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x2a	@ PACKER: left in the buffer by BattleFx_ThornSheet+0x21b
	.endif
