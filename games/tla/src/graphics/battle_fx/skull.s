	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_SkullSheet
BattleFx_SkullSheet:
	.incbin "games/tla/src/graphics/battle_fx/skull.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/skull.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x98, 0x43, 0x4a	@ PACKER: left in the buffer by BattleFx_SheepSheet+0x421
	.else
	.byte 0x6c, 0x09, 0x63	@ PACKER: left in the buffer by BattleFx_PortalSheet+0x421
	.endif
