	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_YellowOrbSheet
BattleFx_YellowOrbSheet:
	.incbin "graphics/battle_fx/yellow_orb.gbapal"
	.incbin "graphics/battle_fx/yellow_orb.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x01, 0x9d	@ PACKER: left in the buffer by BattleFx_LavaOrbSheet+0xa12
	.endif
