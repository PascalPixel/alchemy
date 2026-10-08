	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_LimeColumnSheet
BattleFx_LimeColumnSheet:
	.incbin "graphics/battle_fx/lime_column.gbapal"
	.incbin "graphics/battle_fx/lime_column.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x02	@ PACKER: left in the buffer by BattleFx_RevealMaskF+0x3cf
	.endif
