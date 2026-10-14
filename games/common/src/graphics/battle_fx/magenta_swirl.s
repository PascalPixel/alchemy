	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_MagentaSwirlSheet
BattleFx_MagentaSwirlSheet:
	.incbin "games/common/src/graphics/battle_fx/magenta_swirl.gbapal"
	.incbin "games/common/src/graphics/battle_fx/magenta_swirl.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x1c, 0x4c	@ PACKER: left in the buffer by BattleFx_ExplosionSheet+0x82a
	.endif
