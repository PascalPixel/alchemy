	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_TwoPairBanner
Slots_TwoPairBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/slots/two_pair_ja.gbapal"
	.incbin "games/tla/src/graphics/slots/two_pair_ja.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/two_pair_de.gbapal"
	.incbin "games/tla/src/graphics/slots/two_pair_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/two_pair_es.gbapal"
	.incbin "games/tla/src/graphics/slots/two_pair_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/two_pair_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/two_pair_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/two_pair_it.gbapal"
	.incbin "games/tla/src/graphics/slots/two_pair_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/slots/two_pair.gbapal"
	.incbin "games/tla/src/graphics/slots/two_pair.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x04, 0xa6, 0xc6	@ PACKER: left in the buffer by Slots_GreenDigits+0x579
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x4c, 0x01, 0x8f	@ PACKER: left in the buffer by Slots_GreenDigits+0x6e5
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x04	@ PACKER: left in the buffer by Slots_GreenDigits+0x5e3
	.endif
	.endif
	.endif
	.endif
