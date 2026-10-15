	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_SlashSheet
BattleFx_SlashSheet:
	.incbin "games/common/src/graphics/battle_fx/slash_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/slash.parts.lz"
	.ifeq GAME_TLA
	.byte 0xf1, 0x59	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0xd12
	.else
	.byte 0xf1, 0x59	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0xd12
	.endif
