	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_JupiterDjinnSheet
BattleFx_JupiterDjinnSheet:
	.incbin "graphics/battle_fx/jupiter_djinn.gbapal"
	.incbin "graphics/battle_fx/jupiter_djinn.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x13	@ PACKER: left in the buffer by BattleFx_MarsDjinnSheet+0x3a3
	.endif
