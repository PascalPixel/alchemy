	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_StarDotSheet
BattleFx_StarDotSheet:
	.incbin "games/common/src/graphics/battle_fx/star_dot_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/star_dot.parts.lz"
	.ifne GAME_TLA
	.byte 0x01, 0xba	@ PACKER: left in the buffer by BattleFx_RockWallSheet+0x89e
	.endif
