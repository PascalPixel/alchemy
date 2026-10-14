	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_RuneSheet
BattleFx_RuneSheet:
	.incbin "games/common/src/graphics/battle_fx/rune.gbapal"
	.incbin "games/common/src/graphics/battle_fx/rune.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x43, 0x21, 0xbe	@ PACKER: left in the buffer by BattleFx_SwordSlashSheet+0x3d9
	.endif
