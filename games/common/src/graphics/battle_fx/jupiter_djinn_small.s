	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_JupiterDjinnSmallSheet
BattleFx_JupiterDjinnSmallSheet:
	.incbin "graphics/battle_fx/jupiter_djinn_small.gbapal"
	.incbin "graphics/battle_fx/jupiter_djinn_small.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x0b, 0x1f	@ PACKER: left in the buffer by BattleFx_JupiterDjinnSheet+0x25a
	.endif
