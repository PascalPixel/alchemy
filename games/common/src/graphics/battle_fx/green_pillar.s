	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_GreenPillarSheet
BattleFx_GreenPillarSheet:
	.incbin "games/common/src/graphics/battle_fx/green_pillar_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/green_pillar.parts.lz"
	.ifeq GAME_TLA
	.byte 0x21, 0xf0, 0x28	@ PACKER: left in the buffer by BattleFx_WhirlwindSheet+0x75d
	.endif
