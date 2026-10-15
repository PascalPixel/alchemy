	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_PinkStarSheet
BattleFx_PinkStarSheet:
	.incbin "games/common/src/graphics/battle_fx/pink_star.gbapal"
	.incbin "games/common/src/graphics/battle_fx/pink_star.bitmap.lz"
	.byte 0x80	@ PACKER: left in the buffer by BattleFx_TargetBurstImage+0x38f
