	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_HeartSheet
BattleFx_HeartSheet:
	.incbin "games/common/src/graphics/battle_fx/heart.gbapal"
	.incbin "games/common/src/graphics/battle_fx/heart.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x09	@ PACKER: left in the buffer by BattleFx_SkullSheet+0x297
	.else
	.byte 0x09	@ PACKER: left in the buffer by BattleFx_SkullSheet+0x297
	.endif
