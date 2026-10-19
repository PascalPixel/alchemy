	.include "games/tla/include/game/ed_asm.h"
	.section .rodata
	.global Slots_OnePairBanner
Slots_OnePairBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/slots/one_pair_ja.gbapal"
	.incbin "games/tla/src/graphics/slots/one_pair_ja.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/one_pair_de.gbapal"
	.incbin "games/tla/src/graphics/slots/one_pair_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/one_pair_es.gbapal"
	.incbin "games/tla/src/graphics/slots/one_pair_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/one_pair_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/one_pair_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/one_pair_it.gbapal"
	.incbin "games/tla/src/graphics/slots/one_pair_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/slots/one_pair.gbapal"
	.incbin "games/tla/src/graphics/slots/one_pair.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x34, 0x5e, 0x57	@ PACKER: left in the buffer by Slots_GreenDigits+0x2f9
	.else
	.ifdef TLA_EDITION_EN
	.byte 0xb2, 0x32, 0xe8	@ PACKER: left in the buffer by Slots_GreenDigits+0x3c9
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x63, 0x27	@ PACKER: left in the buffer by Slots_GreenDigits+0x46a
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x67, 0xb6	@ PACKER: left in the buffer by Slots_GreenDigits+0x4de
	.endif
	.endif
	.endif
	.endif
