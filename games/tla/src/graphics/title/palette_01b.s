	.include "games/common/include/game/ed_asm.h"
	.section .rodata
@ 256 色 (言語ごと)
	.global Resource_Data01B
Resource_Data01B:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/title/palette_01b_ja.gbapal"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/title/palette_01b_de.gbapal"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/title/palette_01b_es.gbapal"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/title/palette_01b_fr.gbapal"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/title/palette_01b_it.gbapal"
	.else
	.incbin "games/tla/src/graphics/title/palette_01b.gbapal"
	.endif
	.endif
	.endif
	.endif
	.endif
