	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_FireSwirlSheet
BattleFx_FireSwirlSheet:
	.incbin "games/common/src/graphics/battle_fx/fire_swirl_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/fire_swirl.parts.lz"
	.ifeq GAME_TLA
	.byte 0x12, 0x21	@ PACKER: left in the buffer by BattleFx_FlareSheet+0x442
	.endif
