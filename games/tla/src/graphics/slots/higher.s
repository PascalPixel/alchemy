	.include "games/tla/include/game/ed_asm.h"
	.section .rodata
	.global Slots_HigherBanner
Slots_HigherBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/slots/higher_ja.gbapal"
	.incbin "games/tla/src/graphics/slots/higher_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/higher_de.gbapal"
	.incbin "games/tla/src/graphics/slots/higher_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/higher_es.gbapal"
	.incbin "games/tla/src/graphics/slots/higher_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/higher_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/higher_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/higher_it.gbapal"
	.incbin "games/tla/src/graphics/slots/higher_it.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/slots/higher.gbapal"
	.incbin "games/tla/src/graphics/slots/higher.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0xad, 0xcb, 0x71	@ PACKER: left in the buffer by Slots_YouLoseBanner+0x2ad
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x34, 0x2f	@ PACKER: left in the buffer by Slots_YouLoseBanner+0x27e
	.else
	.ifdef TLA_EDITION_FR
	.byte 0xd8, 0xdd, 0x51	@ PACKER: left in the buffer by Slots_YouLoseBanner+0x299
	.endif
	.endif
	.endif
