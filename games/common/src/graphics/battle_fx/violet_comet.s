	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_VioletCometSheet
BattleFx_VioletCometSheet:
	.incbin "graphics/battle_fx/violet_comet_00.gbapal"
	.incbin "graphics/battle_fx/violet_comet.parts.lz"
	.ifeq GAME_TLA
	.byte 0x5e, 0xda, 0x33	@ PACKER: left in the buffer by BattleFx_FirePillarSheetA+0x3e5
	.endif
