	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_PaletteRampImage
BattleFx_PaletteRampImage:
	.incbin "games/tla/src/graphics/battle_fx/spark_cells.parts.lz"
	.ifne GAME_TLA
	.byte 0x01, 0x0d	@ PACKER: left in the buffer by BattleFx_DaggerSheet+0x196
	.endif
