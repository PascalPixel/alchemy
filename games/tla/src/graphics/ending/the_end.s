	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Ending_TheEndPicture
Ending_TheEndPicture:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/ending/the_end_ja.gbapal"
	.incbin "graphics/ending/the_end_ja.bitmap.d7"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/ending/the_end_de.gbapal"
	.incbin "graphics/ending/the_end_de.bitmap.d7"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/ending/the_end_es.gbapal"
	.incbin "graphics/ending/the_end_es.bitmap.d7"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/ending/the_end_es.gbapal"
	.incbin "graphics/ending/the_end_es.bitmap.d7"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/ending/the_end_it.gbapal"
	.incbin "graphics/ending/the_end_it.bitmap.d7"
	.else
	.incbin "graphics/ending/the_end.gbapal"
	.incbin "graphics/ending/the_end.bitmap.d7"
	.endif
	.endif
	.endif
	.endif
	.endif
