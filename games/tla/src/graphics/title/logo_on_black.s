	.include "games/common/include/game/ed_asm.h"
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
