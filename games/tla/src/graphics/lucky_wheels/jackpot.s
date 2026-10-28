	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global Slots_JackpotBanner
Slots_JackpotBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot_es.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot.4bpp32x32.lz"
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0xa2	@ PACKER: left in the buffer by Slots_Icons+0x66b
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x39	@ PACKER: left in the buffer by Slots_Icons+0x64b
	.else
	.byte 0x75	@ PACKER: left in the buffer by Slots_Icons+0x61b
	.endif
	.endif
