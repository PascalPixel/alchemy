	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_FireStreakSheet
BattleFx_FireStreakSheet:
	.incbin "games/common/src/graphics/battle_fx/fire_streak.gbapal"
	.incbin "games/common/src/graphics/battle_fx/fire_streak.bitmap.lz"
	.byte 0x00, 0x48	@ PACKER: left in the buffer by BattleFx_WhirlwindSheet+0x15c2
