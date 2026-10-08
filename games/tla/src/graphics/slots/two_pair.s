	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_TwoPairBanner
Slots_TwoPairBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/slots/two_pair_ja.gbapal"
	.incbin "graphics/slots/two_pair_ja.4bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/slots/two_pair_de.gbapal"
	.incbin "graphics/slots/two_pair_de.4bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/two_pair_es.gbapal"
	.incbin "graphics/slots/two_pair_es.4bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/slots/two_pair_fr.gbapal"
	.incbin "graphics/slots/two_pair_fr.4bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/slots/two_pair_it.gbapal"
	.incbin "graphics/slots/two_pair_it.4bpp.lz"
	.else
	.incbin "graphics/slots/two_pair.gbapal"
	.incbin "graphics/slots/two_pair.4bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
