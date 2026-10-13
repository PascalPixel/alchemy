	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_TripleBanner
Slots_TripleBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/slots/triple_ja.gbapal"
	.incbin "graphics/slots/triple_ja.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/slots/triple_de.gbapal"
	.incbin "graphics/slots/triple_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/triple_es.gbapal"
	.incbin "graphics/slots/triple_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/slots/triple_fr.gbapal"
	.incbin "graphics/slots/triple_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/slots/triple_it.gbapal"
	.incbin "graphics/slots/triple_it.4bpp64x64.lz"
	.else
	.incbin "graphics/slots/triple.gbapal"
	.incbin "graphics/slots/triple.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
