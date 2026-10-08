	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_DoubleUpBanner
Slots_DoubleUpBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/slots/double_up_ja.gbapal"
	.incbin "graphics/slots/double_up_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/slots/double_up_de.gbapal"
	.incbin "graphics/slots/double_up_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/double_up_es.gbapal"
	.incbin "graphics/slots/double_up_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/slots/double_up_fr.gbapal"
	.incbin "graphics/slots/double_up_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/slots/double_up_it.gbapal"
	.incbin "graphics/slots/double_up_it.4bpp32x32.lz"
	.else
	.incbin "graphics/slots/double_up.gbapal"
	.incbin "graphics/slots/double_up.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
