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
