	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_MercuryLighthouseBackdrop
Battle_MercuryLighthouseBackdrop:
	.incbin "games/tla/src/graphics/backdrop/mercury_lighthouse.gbapal"
	.incbin "games/tla/src/graphics/backdrop/mercury_lighthouse.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x5d, 0xf5	@ PACKER: left in the buffer by Battle_GomaCaveBackdrop+0x4032
	.else
	.byte 0xb8, 0xf6	@ PACKER: left in the buffer by Battle_TretTreeInteriorsBackdrop+0x4032
	.endif
