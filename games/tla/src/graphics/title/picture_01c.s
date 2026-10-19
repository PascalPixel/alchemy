	.include "games/tla/include/game/ed_asm.h"
	.section .rodata
@ The subtitle's words in each language, each sheet three 64x64 word pieces
@ placed side by side in one row, as the title shows them, in four stages
@ from blurred (01C) to sharp (01F, in picture_01e.s), in Resource_Data01B's
@ colours.
@ Streams are rounded to four bytes; PACKER rows name the buffer's last writer.
@ After 01D, French and Italian end with zero fill no earlier stream left;
@ Spanish's bytes are neither its last writer's nor zeros, and stay on its
@ scaffold.
	.ifeq EDITION_INTERNATIONAL

	.global Resource_Data01C
Resource_Data01C:
	.incbin "games/tla/src/graphics/title/picture_01c_ja.8bpp64x64.plz"
	.global Resource_Data01D
Resource_Data01D:
	.incbin "games/tla/src/graphics/title/picture_01d_ja.8bpp64x64.plz"

	.else
	.ifdef TLA_EDITION_DE

	.global Resource_Data01C
Resource_Data01C:
	.incbin "games/tla/src/graphics/title/picture_01c_de.8bpp64x64.plz"
	.global Resource_Data01D
Resource_Data01D:
	.incbin "games/tla/src/graphics/title/picture_01d_de.8bpp64x64.plz"
	.byte 0x34, 0x40	@ PACKER: left in the buffer by Title_IntroGullSprites+0x8fa

	.else
	.ifdef TLA_EDITION_ES

	.global Resource_Data01C
Resource_Data01C:
	.incbin "games/tla/src/graphics/title/picture_01c_es.8bpp64x64.plz"
	.byte 0xe0, 0xdf	@ PACKER: left in the buffer by Title_IntroGullSprites+0x87e
	.global Resource_Data01D
Resource_Data01D:
	.incbin "games/tla/src/graphics/title/picture_01d_es.8bpp64x64.plz"

	.else
	.ifdef TLA_EDITION_FR

	.global Resource_Data01C
Resource_Data01C:
	.incbin "games/tla/src/graphics/title/picture_01c_fr.8bpp64x64.plz"
	.byte 0xf5, 0xa1	@ PACKER: left in the buffer by Title_IntroGullSprites+0x826
	.global Resource_Data01D
Resource_Data01D:
	.incbin "games/tla/src/graphics/title/picture_01d_fr.8bpp64x64.plz"

	.else
	.ifdef TLA_EDITION_IT

	.global Resource_Data01C
Resource_Data01C:
	.incbin "games/tla/src/graphics/title/picture_01c_it.8bpp64x64.plz"
	.byte 0x1c	@ PACKER: left in the buffer by Title_IntroGullSprites+0x807
	.global Resource_Data01D
Resource_Data01D:
	.incbin "games/tla/src/graphics/title/picture_01d_it.8bpp64x64.plz"

	.else

	.global Resource_Data01C
Resource_Data01C:
	.incbin "games/tla/src/graphics/title/picture_01c.8bpp64x64.plz"
	.byte 0xd0, 0xdf	@ PACKER: left in the buffer by Title_IntroGullSprites+0x76e
	.global Resource_Data01D
Resource_Data01D:
	.incbin "games/tla/src/graphics/title/picture_01d.8bpp64x64.plz"
	.byte 0x1c, 0x07, 0x40	@ PACKER: left in the buffer by Title_IntroGullSprites+0x981

	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_FR
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_IT
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
