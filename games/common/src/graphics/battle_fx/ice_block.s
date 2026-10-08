	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "graphics/battle_fx/ice_block_00.gbapal"
	.incbin "graphics/battle_fx/ice_block.parts.lz"
	.ifeq GAME_TLA
	.byte 0x30, 0xae	@ PACKER: left in the buffer by BattleFx_BlueBeastSheet+0x1586
	.endif
