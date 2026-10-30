	.include "games/tbs/include/ed_asm.h"
	.section .rodata
	.global gFont_Glyphs
gFont_Glyphs:
	.ifeq EDITION_INTERNATIONAL
	.ifdef TLA_EDITION_JA
	.incbin "games/tbs/src/graphics/font/text_tla_ja.font", 0, (256 - 32) * (2 + 12 * 2)
	.global gFont_KanjiGlyphs
gFont_KanjiGlyphs:
	.incbin "games/tbs/src/graphics/font/text_tla_ja.font", (256 - 32) * (2 + 12 * 2)
	.else
	.incbin "games/tbs/src/graphics/font/text_ja.font", 0, (256 - 32) * (2 + 12 * 2)
	.global gFont_KanjiGlyphs
gFont_KanjiGlyphs:
	.incbin "games/tbs/src/graphics/font/text_ja.font", (256 - 32) * (2 + 12 * 2)
	.endif
	.else
	.ifdef TBS_EDITION_DE
	.incbin "games/tbs/src/graphics/font/text_de.font"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/font/text_es.font"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/font/text_fr.font"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/font/text_it.font"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tbs/src/graphics/font/text_tla_de.font"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tbs/src/graphics/font/text_tla_es.font"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tbs/src/graphics/font/text_tla_fr.font"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tbs/src/graphics/font/text_tla_it.font"
	.else
	.incbin "games/tbs/src/graphics/font/text.font"
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
