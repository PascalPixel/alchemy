	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_SmokeSheet
BattleFx_SmokeSheet:
	.incbin "games/tla/src/graphics/battle_fx/smoke.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/smoke.bitmap.lz"
	.ifne GAME_TLA
	.byte 0xc1	@ PACKER: left in the buffer by BattleFx_DaggerSheet+0x88b
	.endif
