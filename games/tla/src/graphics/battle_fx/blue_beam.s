	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_BlueBeamSheet
BattleFx_BlueBeamSheet:
	.incbin "games/tla/src/graphics/battle_fx/blue_beam_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/blue_beam.parts.lz"
	.ifne GAME_TLA
	.byte 0x01, 0x53, 0x33	@ PACKER: left in the buffer by BattleFx_WindStreakSheet+0x6b5
	.endif
