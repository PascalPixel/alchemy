	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_TornadoSheet
BattleFx_TornadoSheet:
	.incbin "games/common/src/graphics/battle_fx/tornado_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/tornado.parts.lz"
	.ifne GAME_TLA
	.byte 0x7f, 0x02	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x1b32
	.endif
