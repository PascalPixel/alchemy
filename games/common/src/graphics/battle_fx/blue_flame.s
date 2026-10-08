	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_BlueFlameSheet
BattleFx_BlueFlameSheet:
	.incbin "graphics/battle_fx/blue_flame_00.gbapal"
	.incbin "graphics/battle_fx/blue_flame.parts.lz"
	.ifeq GAME_TLA
	.byte 0xb7, 0x7c	@ PACKER: left in the buffer by BattleFx_WaterSpraySheet+0xaee
	.endif
