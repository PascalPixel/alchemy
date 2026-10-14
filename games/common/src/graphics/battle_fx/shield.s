	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_ShieldSheet
BattleFx_ShieldSheet:
	.incbin "games/common/src/graphics/battle_fx/shield.gbapal"
	.incbin "games/common/src/graphics/battle_fx/shield.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x45, 0xb1, 0x12	@ PACKER: left in the buffer by BattleFx_FireStreakSheet+0xcad
	.endif
