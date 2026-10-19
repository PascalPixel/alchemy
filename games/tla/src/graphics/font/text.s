	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global gUiText_Glyphs
gUiText_Glyphs:
	.ifeq EDITION_INTERNATIONAL
	.ifdef TLA_EDITION_JA
	.incbin "games/tla/src/graphics/font/text_tla_ja.font", 0, (0x100 - 0x20) * (2 + 12 * 2)
	.global gUiText_KanjiGlyphs
gUiText_KanjiGlyphs:
	.incbin "games/tla/src/graphics/font/text_tla_ja.font", (0x100 - 0x20) * (2 + 12 * 2)
	.else
	.incbin "games/tla/src/graphics/font/text_ja.font", 0, (0x100 - 0x20) * (2 + 12 * 2)
	.global gUiText_KanjiGlyphs
gUiText_KanjiGlyphs:
	.incbin "games/tla/src/graphics/font/text_ja.font", (0x100 - 0x20) * (2 + 12 * 2)
	.endif
	.else
	.ifdef TBS_EDITION_DE
	.incbin "games/tla/src/graphics/font/text_de.font"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tla/src/graphics/font/text_es.font"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tla/src/graphics/font/text_fr.font"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tla/src/graphics/font/text_it.font"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/font/text_tla_de.font"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/font/text_tla_es.font"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/font/text_tla_fr.font"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/font/text_tla_it.font"
	.else
	.incbin "games/tla/src/graphics/font/text.font"
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
