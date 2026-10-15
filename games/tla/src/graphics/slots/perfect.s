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
	.ifdef TLA_EDITION_JA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_EN
	.byte 0xc3	@ PACKER: left in the buffer by Slots_GreenDigits+0x663
	.else
	.ifdef TLA_EDITION_DE
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x02	@ PACKER: left in the buffer by Resource_Data087+0x7ab
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xe2, 0x3c, 0x30	@ PACKER: left in the buffer by Slots_GreenDigits+0x64d
	.endif
	.endif
	.endif
	.endif
	.endif
