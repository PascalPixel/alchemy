	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_SmallDoubleUpBanner
Slots_SmallDoubleUpBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/slots/small_double_up_ja.gbapal"
	.incbin "games/tla/src/graphics/slots/small_double_up_ja.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/small_double_up_de.gbapal"
	.incbin "games/tla/src/graphics/slots/small_double_up_de.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/small_double_up_es.gbapal"
	.incbin "games/tla/src/graphics/slots/small_double_up_es.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/small_double_up_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/small_double_up_fr.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/small_double_up_it.gbapal"
	.incbin "games/tla/src/graphics/slots/small_double_up_it.4bpp16x16.lz"
	.else
	.incbin "games/tla/src/graphics/slots/small_double_up.gbapal"
	.incbin "games/tla/src/graphics/slots/small_double_up.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x5e, 0xe3, 0x09	@ PACKER: left in the buffer by Slots_GreenDigits+0x19d
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x1d, 0x98	@ PACKER: left in the buffer by Slots_GreenDigits+0x162
	.else
	.ifdef TLA_EDITION_FR
	.else
	.ifdef TLA_EDITION_IT
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
	.endif
	.endif
	.endif
