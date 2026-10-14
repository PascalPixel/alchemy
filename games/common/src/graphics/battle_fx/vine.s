	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_VineSheet
BattleFx_VineSheet:
	.incbin "games/common/src/graphics/battle_fx/vine_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/vine.parts.lz"
	.ifeq GAME_TLA
	.byte 0xbe	@ PACKER: left in the buffer by BattleFx_WhirlwindSheet+0xdcb
	.endif
