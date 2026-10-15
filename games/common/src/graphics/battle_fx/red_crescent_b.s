	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_RedCrescentSheetB
BattleFx_RedCrescentSheetB:
	.incbin "games/common/src/graphics/battle_fx/red_crescent_b.gbapal"
	.incbin "games/common/src/graphics/battle_fx/red_crescent_b.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0xf0, 0x79	@ PACKER: left in the buffer by BattleFx_BlueBeastSheet+0x3ea
	.else
	.byte 0xf0, 0x79	@ PACKER: left in the buffer by BattleFx_BlueBeastSheet+0x3ea
	.endif
