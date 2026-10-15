	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_GreenVineSheet
BattleFx_GreenVineSheet:
	.incbin "games/common/src/graphics/battle_fx/green_vine.gbapal"
	.incbin "games/common/src/graphics/battle_fx/green_vine.bitmap.lz"
	.byte 0x1c, 0xaf	@ PACKER: left in the buffer by BattleFx_WispSheet+0x516
