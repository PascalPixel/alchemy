	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global Title_IntroLighthousePicture
Title_IntroLighthousePicture:
	.incbin "games/tla/src/graphics/title/intro_lighthouse.gbapal"
	.incbin "games/tla/src/graphics/title/intro_lighthouse.8bpp.plz"
	.balign 4, 0
	.global Title_IntroGullSprites
Title_IntroGullSprites:
	.incbin "games/tla/src/graphics/title/intro_gulls.gbapal"
	.incbin "games/tla/src/graphics/title/intro_gulls.4bpp.plz"
	.byte 0x19, 0x2f, 0x63	@ PACKER: left in the buffer by Title_IntroLighthousePicture+0x11ed
	.section .rodata.a
	.global Title_NintendoLogo
Title_NintendoLogo:
	.incbin "games/tla/src/graphics/title/nintendo_splash.gbapal"
	.incbin "games/tla/src/graphics/title/nintendo_splash.8bpp.lz"
@ The "CAMELOT SOFTWARE PLANNING" logo and four flag frames.
	.global Title_CamelotLogo
Title_CamelotLogo:
	.incbin "games/tla/src/graphics/title/camelot_logo.parts.lz"
	.section .rodata.b
	.global Resource_Data023
Resource_Data023:
	.incbin "games/tla/src/graphics/title/picture_023.strips"
	.byte 0x8f	@ PACKER: left in the buffer by Title_IntroLighthousePicture+0x761b
	.global Resource_Data024
Resource_Data024:
	.incbin "games/tla/src/graphics/title/picture_024.strips"
@ A copy of The Broken Seal's Title_IntroTilesB, the "To be continued" lettering.
	.global Resource_Data025
Resource_Data025:
	.incbin "games/tla/src/graphics/title/to_be_continued.parts.lz"
	.section .rodata.c
	.global Title_ScreenPicture
Title_ScreenPicture:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/title/title_screen_ja.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen_ja.8bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/title/title_screen_de.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen_de.8bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/title/title_screen_es.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen_es.8bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/title/title_screen_fr.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen_fr.8bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/title/title_screen_it.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen_it.8bpp.lz"
	.else
	.incbin "games/tla/src/graphics/title/title_screen.gbapal"
	.incbin "games/tla/src/graphics/title/title_screen.8bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifndef TLA_EDITION_ES
	.ifndef TLA_EDITION_FR
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
	.balign 4, 0
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "games/tla/src/graphics/title/intro_peak.gbapal"
	.incbin "games/tla/src/graphics/title/intro_peak.parts.plz"
	.balign 4, 0
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
	.section .rodata.d
@ 256 colors, per language.
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
@ The subtitle's words in each language, each sheet three 64x64 word pieces
@ placed side by side in one row, as the title shows them, in four stages
@ from blurred (01C) to sharp (01F, in picture_01e.s), in Resource_Data01B's
@ colours.
@ Streams are rounded to four bytes; PACKER rows name the buffer's last writer.
@ After 01D, French and Italian end with zero fill no earlier stream left;
@ Spanish's bytes are neither its last writer's nor zeros: residue whose
@ writer is unknown.
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
	.ifdef TLA_EDITION_ES
	.byte 0x00, 0xe0	@ PACKER: unused post-stream residue; Resource_DecodeType01 uses decoded RAM and output length; writer unknown
	.else
	.ifdef TLA_EDITION_FR
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_IT
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
	.endif
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
