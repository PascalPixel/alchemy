	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_RockSpireSheet
BattleFx_RockSpireSheet:
	.incbin "games/common/src/graphics/battle_fx/rock_spire.gbapal"
	.incbin "games/common/src/graphics/battle_fx/rock_spire.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0xa0, 0x20, 0x72	@ PACKER: left in the buffer by BattleFx_FirePillarSheetB+0x7a1
	.endif
