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
	.ifdef TLA_EDITION_JA
	.byte 0x88, 0x0e	@ PACKER: left in the buffer by Ending_SunsetPlainPicture+0x5cd2
	.else
	.ifdef TLA_EDITION_EN
	.byte 0xc5, 0x19	@ PACKER: left in the buffer by Ending_SunsetPlainPicture+0x5d1e
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x00, 0x00	@ PACKER: left in the buffer by Ending_SunsetPlainPicture+0x59b6
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x50, 0x45	@ PACKER: left in the buffer by Ending_SunsetPlainPicture+0x5d42
	.endif
	.endif
	.endif
	.endif
