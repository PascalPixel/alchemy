	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_PerfectBanner
Slots_PerfectBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/slots/perfect_ja.gbapal"
	.incbin "games/tla/src/graphics/slots/perfect_ja.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/perfect_de.gbapal"
	.incbin "games/tla/src/graphics/slots/perfect_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/perfect_es.gbapal"
	.incbin "games/tla/src/graphics/slots/perfect_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/perfect_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/perfect_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/perfect_it.gbapal"
	.incbin "games/tla/src/graphics/slots/perfect_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/slots/perfect.gbapal"
	.incbin "games/tla/src/graphics/slots/perfect.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
