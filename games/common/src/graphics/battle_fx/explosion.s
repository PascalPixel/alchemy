	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_ExplosionSheet
BattleFx_ExplosionSheet:
	.incbin "graphics/battle_fx/explosion.gbapal"
	.incbin "graphics/battle_fx/explosion.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0xec, 0x10, 0x3c	@ PACKER: left in the buffer by BattleFx_WaterSpraySheet+0xdd5
	.endif
