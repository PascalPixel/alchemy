	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_FlameColumnSheet
BattleFx_FlameColumnSheet:
	.incbin "graphics/battle_fx/flame_column.gbapal"
	.incbin "graphics/battle_fx/flame_column.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x41	@ PACKER: left in the buffer by BattleFx_RuneSheet+0x3ab
	.endif
