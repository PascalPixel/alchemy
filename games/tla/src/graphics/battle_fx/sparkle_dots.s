	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_SparkleDots
BattleFx_SparkleDots:
	.incbin "games/tla/src/graphics/battle_fx/sparkle_dots.parts.lz"
	.ifne GAME_TLA
	.byte 0x63, 0x34, 0x77	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x61
	.endif
