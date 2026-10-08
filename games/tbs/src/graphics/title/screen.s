	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Title_IntroGraphicsB
Title_IntroGraphicsB:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/title/screen_ja.gbapal"
	.incbin "graphics/title/screen_ja.8bpp.plz"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "graphics/title/screen_de.gbapal"
	.incbin "graphics/title/screen_de.8bpp.plz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "graphics/title/screen_de.gbapal"
	.incbin "graphics/title/screen_de.8bpp.plz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "graphics/title/screen_de.gbapal"
	.incbin "graphics/title/screen_de.8bpp.plz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "graphics/title/screen_de.gbapal"
	.incbin "graphics/title/screen_de.8bpp.plz"
	.else
	.incbin "graphics/title/screen.gbapal"
	.incbin "graphics/title/screen.8bpp.plz"
	.endif
	.endif
	.endif
	.endif
	.endif
