	.include "games/tla/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_RockWallSheet
BattleFx_RockWallSheet:
	.incbin "games/tla/src/graphics/battle_fx/rock_wall.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/rock_wall.bitmap.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x07, 0x43, 0x0b	@ PACKER: left in the buffer by BattleFx_VineSheet+0xcd1
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
