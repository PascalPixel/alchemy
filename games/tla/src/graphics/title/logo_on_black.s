	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Title_LogoOnBlackPicture
Title_LogoOnBlackPicture:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/title/logo_on_black_ja.gbapal"
	.incbin "graphics/title/logo_on_black_ja.8bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/title/logo_on_black_de.gbapal"
	.incbin "graphics/title/logo_on_black_de.8bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/title/logo_on_black_es.gbapal"
	.incbin "graphics/title/logo_on_black_es.8bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/title/logo_on_black_fr.gbapal"
	.incbin "graphics/title/logo_on_black_fr.8bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/title/logo_on_black_it.gbapal"
	.incbin "graphics/title/logo_on_black_it.8bpp.lz"
	.else
	.incbin "graphics/title/logo_on_black.gbapal"
	.incbin "graphics/title/logo_on_black.8bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
