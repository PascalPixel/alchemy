	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_DuskCloudsBackdrop
Battle_DuskCloudsBackdrop:
	.incbin "games/common/src/graphics/backdrop/dusk_clouds.gbapal"
	.incbin "games/common/src/graphics/backdrop/dusk_clouds.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x33, 0x00	@ PACKER: left in the buffer by Battle_ForestBackdrop+0x39aa
	.endif
