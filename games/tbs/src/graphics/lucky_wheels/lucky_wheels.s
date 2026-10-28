	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.global LuckyWheels_MachinePicture
LuckyWheels_MachinePicture:
	.incbin "games/tbs/src/graphics/lucky_wheels/machine.gbapal"
	.incbin "games/tbs/src/graphics/lucky_wheels/machine.4bpp.lz"
	.byte 0x0f, 0x8b, 0xa4	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x781
	.global LuckyWheels_Icons
LuckyWheels_Icons:
	.incbin "games/tbs/src/graphics/lucky_wheels/icons.gbapal"
	.incbin "games/tbs/src/graphics/lucky_wheels/icons.8bpp.lz"
	.global LuckyWheels_Jackpot
LuckyWheels_Jackpot:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tbs/src/graphics/lucky_wheels/jackpot_ja.gbapal"
	.incbin "games/tbs/src/graphics/lucky_wheels/jackpot_ja.4bpp32x32.lz"
	.else
	.incbin "games/tbs/src/graphics/lucky_wheels/jackpot.gbapal"
	.incbin "games/tbs/src/graphics/lucky_wheels/jackpot.4bpp32x32.lz"
	.endif
	.ifdef TBS_EDITION_JA
	.byte 0xa2	@ PACKER: left in the buffer by LuckyWheels_Icons+0x66b
	.else
	.byte 0x39	@ PACKER: left in the buffer by LuckyWheels_Icons+0x64b
	.endif
