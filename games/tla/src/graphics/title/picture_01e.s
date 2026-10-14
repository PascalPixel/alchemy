	.include "games/common/include/game/ed_asm.h"
	.section .rodata
@ The subtitle's words, the two sharper stages (see picture_01c.s).
@ Streams are rounded to four bytes; PACKER rows name the buffer's last writer.
	.ifeq EDITION_INTERNATIONAL

	.global Resource_Data01E
Resource_Data01E:
	.incbin "games/tla/src/graphics/title/picture_01e_ja.8bpp64x64.plz"
	.byte 0x75, 0x08, 0x01	@ PACKER: left in the buffer by Resource_Data01D+0x815
	.global Resource_Data01F
Resource_Data01F:
	.incbin "games/tla/src/graphics/title/picture_01f_ja.8bpp64x64.plz"

	.else
	.ifdef TLA_EDITION_DE

	.global Resource_Data01E
Resource_Data01E:
	.incbin "games/tla/src/graphics/title/picture_01e_de.8bpp64x64.plz"
	.byte 0x49	@ PACKER: left in the buffer by Resource_Data01D+0x8f7
	.global Resource_Data01F
Resource_Data01F:
	.incbin "games/tla/src/graphics/title/picture_01f_de.8bpp64x64.plz"

	.else
	.ifdef TLA_EDITION_ES

	.global Resource_Data01E
Resource_Data01E:
	.incbin "games/tla/src/graphics/title/picture_01e_es.8bpp64x64.plz"
	.global Resource_Data01F
Resource_Data01F:
	.incbin "games/tla/src/graphics/title/picture_01f_es.8bpp64x64.plz"
	.byte 0x03, 0x19, 0xcb	@ PACKER: left in the buffer by Resource_Data01E+0x6a1

	.else
	.ifdef TLA_EDITION_FR

	.global Resource_Data01E
Resource_Data01E:
	.incbin "games/tla/src/graphics/title/picture_01e_fr.8bpp64x64.plz"
	.byte 0x08, 0x18	@ PACKER: left in the buffer by Resource_Data01D+0x7aa
	.global Resource_Data01F
Resource_Data01F:
	.incbin "games/tla/src/graphics/title/picture_01f_fr.8bpp64x64.plz"
	.byte 0x14	@ PACKER: left in the buffer by Resource_Data01E+0x56b

	.else
	.ifdef TLA_EDITION_IT

	.global Resource_Data01E
Resource_Data01E:
	.incbin "games/tla/src/graphics/title/picture_01e_it.8bpp64x64.plz"
	.byte 0xf9	@ PACKER: left in the buffer by Resource_Data01D+0x743
	.global Resource_Data01F
Resource_Data01F:
	.incbin "games/tla/src/graphics/title/picture_01f_it.8bpp64x64.plz"

	.else

	.global Resource_Data01E
Resource_Data01E:
	.incbin "games/tla/src/graphics/title/picture_01e.8bpp64x64.plz"
	.byte 0x02	@ PACKER: left in the buffer by Resource_Data01D+0x877
	.global Resource_Data01F
Resource_Data01F:
	.incbin "games/tla/src/graphics/title/picture_01f.8bpp64x64.plz"
	.byte 0xd5, 0x06	@ PACKER: left in the buffer by Resource_Data01E+0x5d2

	.endif
	.endif
	.endif
	.endif
	.endif
