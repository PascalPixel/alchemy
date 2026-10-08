	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_SmallDoubleUpBanner
Slots_SmallDoubleUpBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/slots/small_double_up_ja.gbapal"
	.incbin "graphics/slots/small_double_up_ja.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/slots/small_double_up_de.gbapal"
	.incbin "graphics/slots/small_double_up_de.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/small_double_up_es.gbapal"
	.incbin "graphics/slots/small_double_up_es.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/slots/small_double_up_fr.gbapal"
	.incbin "graphics/slots/small_double_up_fr.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/slots/small_double_up_it.gbapal"
	.incbin "graphics/slots/small_double_up_it.4bpp16x16.lz"
	.else
	.incbin "graphics/slots/small_double_up.gbapal"
	.incbin "graphics/slots/small_double_up.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
