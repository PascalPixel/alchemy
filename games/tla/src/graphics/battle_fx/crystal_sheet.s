	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_CrystalSheet
BattleFx_CrystalSheet:
	.incbin "games/tla/src/graphics/battle_fx/crystal_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/crystal.parts.lz"
	.ifne GAME_TLA
	.byte 0x13, 0x42, 0x03	@ PACKER: left in the buffer by BattleFx_TornadoSheet+0x104d
	.endif
