	.include "games/tla/include/game/ed_asm.h"
	.section .rodata
	.global Slots_LowerBanner
Slots_LowerBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/slots/lower_ja.gbapal"
	.incbin "games/tla/src/graphics/slots/lower_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/lower_de.gbapal"
	.incbin "games/tla/src/graphics/slots/lower_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/lower_es.gbapal"
	.incbin "games/tla/src/graphics/slots/lower_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/lower_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/lower_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/lower_it.gbapal"
	.incbin "games/tla/src/graphics/slots/lower_it.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/slots/lower.gbapal"
	.incbin "games/tla/src/graphics/slots/lower.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x13	@ PACKER: left in the buffer by Slots_HigherBanner+0x23b
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x90, 0xb8, 0x94	@ PACKER: left in the buffer by Slots_HigherBanner+0x2bd
	.else
	.ifdef TLA_EDITION_FR
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x3a	@ PACKER: left in the buffer by Slots_HigherBanner+0x257
	.endif
	.endif
	.endif
	.endif
