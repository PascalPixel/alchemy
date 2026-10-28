	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global Title_ScreenPicture
Title_ScreenPicture:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/title/title_screen_ja.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen_ja.8bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/title/title_screen_de.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen_de.8bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/title/title_screen_es.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen_es.8bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/title/title_screen_fr.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen_fr.8bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/title/title_screen_it.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen_it.8bpp.lz"
	.else
	.incbin "games/tla/src/graphics/title/title_screen.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen.8bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifndef TLA_EDITION_ES
	.ifndef TLA_EDITION_FR
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
