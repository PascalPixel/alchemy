	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_MarsDjinnSheet
BattleFx_MarsDjinnSheet:
	.incbin "games/common/src/graphics/battle_fx/mars_djinn.gbapal"
	.incbin "games/common/src/graphics/battle_fx/mars_djinn.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x04	@ PACKER: left in the buffer by BattleFx_StarDotSheet+0x3db
	.endif
