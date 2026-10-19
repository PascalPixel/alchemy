	.include "games/tla/include/game/ed_asm.h"
	.section .rodata
	.global Title_LogoOnBlackPicture
Title_LogoOnBlackPicture:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/title/logo_on_black_ja.gbapal"
	.incbin "games/tla/src/graphics/title/logo_on_black_ja.8bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/title/logo_on_black_de.gbapal"
	.incbin "games/tla/src/graphics/title/logo_on_black_de.8bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/title/logo_on_black_es.gbapal"
	.incbin "games/tla/src/graphics/title/logo_on_black_es.8bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/title/logo_on_black_fr.gbapal"
	.incbin "games/tla/src/graphics/title/logo_on_black_fr.8bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/title/logo_on_black_it.gbapal"
	.incbin "games/tla/src/graphics/title/logo_on_black_it.8bpp.lz"
	.else
	.incbin "games/tla/src/graphics/title/logo_on_black.gbapal"
	.incbin "games/tla/src/graphics/title/logo_on_black.8bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x0e, 0x44, 0xc0	@ PACKER: left in the buffer by Title_CamelotLogo+0x153d
	.else
	.ifdef TLA_EDITION_EN
	.byte 0x08	@ PACKER: left in the buffer by Title_CamelotLogo+0x15b7
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x5e, 0x8b	@ PACKER: left in the buffer by Title_CamelotLogo+0x156e
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x3d	@ PACKER: left in the buffer by Title_CamelotLogo+0x1537
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x5d	@ PACKER: left in the buffer by Title_CamelotLogo+0x1527
	.endif
	.endif
	.endif
	.endif
	.endif
