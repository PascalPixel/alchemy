	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_LowerBanner
Slots_LowerBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/slots/lower_ja.gbapal"
	.incbin "graphics/slots/lower_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/slots/lower_de.gbapal"
	.incbin "graphics/slots/lower_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/lower_es.gbapal"
	.incbin "graphics/slots/lower_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/slots/lower_fr.gbapal"
	.incbin "graphics/slots/lower_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/slots/lower_it.gbapal"
	.incbin "graphics/slots/lower_it.4bpp32x32.lz"
	.else
	.incbin "graphics/slots/lower.gbapal"
	.incbin "graphics/slots/lower.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
