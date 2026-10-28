	.include "games/tla/include/ed_asm.h"
	.section .rodata
@ Two cloud images and "Camelot presents / Golden Sun / Book Two" (per language).
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
	.ifdef TLA_EDITION_JA
	.byte 0x08, 0x04	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x56ae
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x08, 0x71	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x531e
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x52, 0x62, 0x04	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x50fd
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xee, 0xac	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x519a
	.endif
	.endif
	.endif
	.endif
