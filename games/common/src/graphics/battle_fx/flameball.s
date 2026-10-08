	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_FlameballSheet
BattleFx_FlameballSheet:
	.incbin "graphics/battle_fx/flameball.gbapal"
	.incbin "graphics/battle_fx/flameball.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x83	@ PACKER: left in the buffer by BattleFx_FirePillarSheetA+0x557
	.endif
