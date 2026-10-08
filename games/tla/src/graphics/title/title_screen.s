	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Title_ScreenPicture
Title_ScreenPicture:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/title/title_screen_ja.gbapal"
	.incbin "graphics/title/title_screen_ja.8bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/title/title_screen_de.gbapal"
	.incbin "graphics/title/title_screen_de.8bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/title/title_screen_es.gbapal"
	.incbin "graphics/title/title_screen_es.8bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/title/title_screen_fr.gbapal"
	.incbin "graphics/title/title_screen_fr.8bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/title/title_screen_it.gbapal"
	.incbin "graphics/title/title_screen_it.8bpp.lz"
	.else
	.incbin "graphics/title/title_screen.gbapal"
	.incbin "graphics/title/title_screen.8bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
