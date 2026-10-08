	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_RockWallSheet
BattleFx_RockWallSheet:
	.incbin "graphics/battle_fx/rock_wall.gbapal"
	.incbin "graphics/battle_fx/rock_wall.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x07, 0x43, 0x0b	@ PACKER: left in the buffer by BattleFx_VineSheet+0xcd1
	.endif
