	.include "games/common/include/game/ed_asm.h"
	.section .rodata
@ 雲二枚と「Camelot presents / Golden Sun / Book Two」の文字 (言語ごと)
	.global Resource_Data018
Resource_Data018:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/title/intro_018/palette_ja.gbapal"
	.incbin "games/tla/src/graphics/title/intro_018/picture_ja.parts.plz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/title/intro_018/palette_de.gbapal"
	.incbin "games/tla/src/graphics/title/intro_018/picture_de.parts.plz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/title/intro_018/palette_es.gbapal"
	.incbin "games/tla/src/graphics/title/intro_018/picture_es.parts.plz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/title/intro_018/palette_fr.gbapal"
	.incbin "games/tla/src/graphics/title/intro_018/picture_fr.parts.plz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/title/intro_018/palette_it.gbapal"
	.incbin "games/tla/src/graphics/title/intro_018/picture_it.parts.plz"
	.else
	.incbin "games/tla/src/graphics/title/intro_018/palette.gbapal"
	.incbin "games/tla/src/graphics/title/intro_018/picture.parts.plz"
	.endif
	.endif
	.endif
	.endif
	.endif
