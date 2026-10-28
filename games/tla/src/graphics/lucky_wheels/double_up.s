	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global Slots_DoubleUpBanner
Slots_DoubleUpBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_it.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/double_up.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_ES
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xbe	@ PACKER: left in the buffer by Slots_YouLoseBanner+0x48f
	.endif
	.endif
	.endif
	.endif
