	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_PortalSheet
BattleFx_PortalSheet:
	.incbin "games/common/src/graphics/battle_fx/portal_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/portal.parts.lz"
	.ifne GAME_TLA
	.byte 0x3f, 0x10	@ PACKER: left in the buffer by BattleFx_WispSheet+0x622
	.endif
