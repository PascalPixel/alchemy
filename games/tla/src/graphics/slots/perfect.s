	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_PerfectBanner
Slots_PerfectBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/slots/perfect_ja.gbapal"
	.incbin "graphics/slots/perfect_ja.4bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/slots/perfect_de.gbapal"
	.incbin "graphics/slots/perfect_de.4bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/perfect_es.gbapal"
	.incbin "graphics/slots/perfect_es.4bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/slots/perfect_fr.gbapal"
	.incbin "graphics/slots/perfect_fr.4bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/slots/perfect_it.gbapal"
	.incbin "graphics/slots/perfect_it.4bpp.lz"
	.else
	.incbin "graphics/slots/perfect.gbapal"
	.incbin "graphics/slots/perfect.4bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
