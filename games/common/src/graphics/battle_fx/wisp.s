	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_WispSheet
BattleFx_WispSheet:
	.incbin "games/common/src/graphics/battle_fx/wisp.gbapal"
	.incbin "games/common/src/graphics/battle_fx/wisp.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x2e, 0x06	@ PACKER: left in the buffer by BattleFx_SmokeSheet+0x7ae
	.else
	.byte 0x2e, 0x06	@ PACKER: left in the buffer by BattleFx_SmokeSheet+0x7ae
	.endif
