	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Ending_TheEndPicture
Ending_TheEndPicture:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/ending/the_end_ja.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end_ja.bitmap.d7"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/ending/the_end_de.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end_de.bitmap.d7"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/ending/the_end_es.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end_es.bitmap.d7"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/ending/the_end_es.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end_es.bitmap.d7"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/ending/the_end_it.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end_it.bitmap.d7"
	.else
	.incbin "games/tla/src/graphics/ending/the_end.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end.bitmap.d7"
	.endif
	.endif
	.endif
	.endif
	.endif
