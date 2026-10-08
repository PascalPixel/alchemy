	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_ThornSheet
BattleFx_ThornSheet:
	.incbin "graphics/battle_fx/thorn_00.gbapal"
	.incbin "graphics/battle_fx/thorn.parts.lz"
	.ifeq GAME_TLA
	.byte 0x78	@ PACKER: left in the buffer by BattleFx_FlashBurstSheet+0xd9b
	.endif
