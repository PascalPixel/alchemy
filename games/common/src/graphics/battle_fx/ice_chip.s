	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_IceChipSheet
BattleFx_IceChipSheet:
	.incbin "games/common/src/graphics/battle_fx/ice_chip_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/ice_chip.parts.lz"
	.ifne GAME_TLA
	.byte 0x04	@ PACKER: left in the buffer by BattleFx_GlowOrbSheet+0x63f
	.endif
