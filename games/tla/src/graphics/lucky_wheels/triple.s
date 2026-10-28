	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global Slots_TripleBanner
Slots_TripleBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/triple_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple_ja.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/triple_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/triple_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/triple_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/triple_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/triple.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x7c, 0x2e, 0xbc	@ PACKER: left in the buffer by Slots_GreenDigits+0x3e1
	.else
	.ifdef TLA_EDITION_EN
	.byte 0x1d, 0xe9	@ PACKER: left in the buffer by Slots_TwoPairBanner+0x396
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x44	@ PACKER: left in the buffer by Slots_TwoPairBanner+0x393
	.endif
	.endif
	.endif
