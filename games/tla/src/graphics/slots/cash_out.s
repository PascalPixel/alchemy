	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_CashOutBanner
Slots_CashOutBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/slots/cash_out_ja.gbapal"
	.incbin "games/tla/src/graphics/slots/cash_out_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/cash_out_de.gbapal"
	.incbin "games/tla/src/graphics/slots/cash_out_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/cash_out_es.gbapal"
	.incbin "games/tla/src/graphics/slots/cash_out_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/cash_out_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/cash_out_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/cash_out_it.gbapal"
	.incbin "games/tla/src/graphics/slots/cash_out_it.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/slots/cash_out.gbapal"
	.incbin "games/tla/src/graphics/slots/cash_out.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x9f, 0x7a, 0xe0	@ PACKER: left in the buffer by Slots_DoubleUpBanner+0x305
	.else
	.ifdef TLA_EDITION_EN
	.byte 0x0f	@ PACKER: left in the buffer by Slots_DoubleUpBanner+0x48f
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x80, 0x4e	@ PACKER: left in the buffer by Slots_DoubleUpBanner+0x3a2
	.else
	.ifdef TLA_EDITION_IT
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
	.endif
	.endif
