	.include "games/tla/include/edition.inc"
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
	.global Title_WorldMapPicture
Title_WorldMapPicture:
	.incbin "games/tla/src/graphics/title/world_map_picture.strips"
	.byte 0x8f	@ PACKER: left in the buffer by Title_IntroLighthousePicture+0x761b
	.global Title_WorldMapPictureLarge
Title_WorldMapPictureLarge:
	.incbin "games/tla/src/graphics/title/world_map_picture_large.strips"
@ A copy of The Broken Seal's Title_IntroTilesB, the "To be continued" lettering.
	.global Title_IntroTilesB
Title_IntroTilesB:
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
	.global Title_IntroPeak
Title_IntroPeak:
	.incbin "games/tla/src/graphics/title/intro_peak.gbapal"
	.incbin "games/tla/src/graphics/title/intro_peak.parts.plz"
	.balign 4, 0
@ Two cloud images and "Camelot presents / Golden Sun / Book Two" (per language).
	.global Title_IntroClouds
Title_IntroClouds:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/title/intro_clouds/palette_ja.gbapal"
	.incbin "games/tla/src/graphics/title/intro_clouds/picture_ja.parts.plz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/title/intro_clouds/palette_de.gbapal"
	.incbin "games/tla/src/graphics/title/intro_clouds/picture_de.parts.plz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/title/intro_clouds/palette_es.gbapal"
	.incbin "games/tla/src/graphics/title/intro_clouds/picture_es.parts.plz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/title/intro_clouds/palette_fr.gbapal"
	.incbin "games/tla/src/graphics/title/intro_clouds/picture_fr.parts.plz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/title/intro_clouds/palette_it.gbapal"
	.incbin "games/tla/src/graphics/title/intro_clouds/picture_it.parts.plz"
	.else
	.incbin "games/tla/src/graphics/title/intro_clouds/palette.gbapal"
	.incbin "games/tla/src/graphics/title/intro_clouds/picture.parts.plz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x08, 0x04	@ PACKER: left in the buffer by Title_IntroPeak+0x56ae
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x08, 0x71	@ PACKER: left in the buffer by Title_IntroPeak+0x531e
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x52, 0x62, 0x04	@ PACKER: left in the buffer by Title_IntroPeak+0x50fd
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xee, 0xac	@ PACKER: left in the buffer by Title_IntroPeak+0x519a
	.endif
	.endif
	.endif
	.endif
	.section .rodata.d
@ 256 colors, per language.
	.global Resource_SubtitlePalette
Resource_SubtitlePalette:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/title/subtitle_palette_ja.gbapal"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/title/subtitle_palette_de.gbapal"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/title/subtitle_palette_es.gbapal"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/title/subtitle_palette_fr.gbapal"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/title/subtitle_palette_it.gbapal"
	.else
	.incbin "games/tla/src/graphics/title/subtitle_palette.gbapal"
	.endif
	.endif
	.endif
	.endif
	.endif
@ The subtitle's words in each language, each sprite sheet three 64x64 word
@ pieces placed side by side in one row, as the title shows them, in four stages
@ from blurred (subtitle_a) to sharp (subtitle_d), in Resource_SubtitlePalette's
@ colours.
@ Streams are rounded to four bytes; PACKER rows name the buffer's last writer.
@ After subtitle_b, French and Italian end with zero fill no earlier stream left;
@ Spanish's bytes are neither its last writer's nor zeros: residue whose
@ writer is unknown.
	.ifeq EDITION_INTERNATIONAL

	.global Resource_SubtitleA
Resource_SubtitleA:
	.incbin "games/tla/src/graphics/title/subtitle_a_ja.8bpp64x64.plz"
	.global Resource_SubtitleB
Resource_SubtitleB:
	.incbin "games/tla/src/graphics/title/subtitle_b_ja.8bpp64x64.plz"

	.else
	.ifdef TLA_EDITION_DE

	.global Resource_SubtitleA
Resource_SubtitleA:
	.incbin "games/tla/src/graphics/title/subtitle_a_de.8bpp64x64.plz"
	.global Resource_SubtitleB
Resource_SubtitleB:
	.incbin "games/tla/src/graphics/title/subtitle_b_de.8bpp64x64.plz"
	.byte 0x34, 0x40	@ PACKER: left in the buffer by Title_IntroGullSprites+0x8fa

	.else
	.ifdef TLA_EDITION_ES

	.global Resource_SubtitleA
Resource_SubtitleA:
	.incbin "games/tla/src/graphics/title/subtitle_a_es.8bpp64x64.plz"
	.byte 0xe0, 0xdf	@ PACKER: left in the buffer by Title_IntroGullSprites+0x87e
	.global Resource_SubtitleB
Resource_SubtitleB:
	.incbin "games/tla/src/graphics/title/subtitle_b_es.8bpp64x64.plz"

	.else
	.ifdef TLA_EDITION_FR

	.global Resource_SubtitleA
Resource_SubtitleA:
	.incbin "games/tla/src/graphics/title/subtitle_a_fr.8bpp64x64.plz"
	.byte 0xf5, 0xa1	@ PACKER: left in the buffer by Title_IntroGullSprites+0x826
	.global Resource_SubtitleB
Resource_SubtitleB:
	.incbin "games/tla/src/graphics/title/subtitle_b_fr.8bpp64x64.plz"

	.else
	.ifdef TLA_EDITION_IT

	.global Resource_SubtitleA
Resource_SubtitleA:
	.incbin "games/tla/src/graphics/title/subtitle_a_it.8bpp64x64.plz"
	.byte 0x1c	@ PACKER: left in the buffer by Title_IntroGullSprites+0x807
	.global Resource_SubtitleB
Resource_SubtitleB:
	.incbin "games/tla/src/graphics/title/subtitle_b_it.8bpp64x64.plz"

	.else

	.global Resource_SubtitleA
Resource_SubtitleA:
	.incbin "games/tla/src/graphics/title/subtitle_a.8bpp64x64.plz"
	.byte 0xd0, 0xdf	@ PACKER: left in the buffer by Title_IntroGullSprites+0x76e
	.global Resource_SubtitleB
Resource_SubtitleB:
	.incbin "games/tla/src/graphics/title/subtitle_b.8bpp64x64.plz"
	.byte 0x1c, 0x07, 0x40	@ PACKER: left in the buffer by Title_IntroGullSprites+0x981

	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x00, 0xe0	@ PACKER: unused post-stream residue; Decompress_TaggedLz uses decoded RAM and output length; writer unknown
	.else
	.ifdef TLA_EDITION_FR
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_IT
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
	.endif
@ The subtitle's words, the two sharper stages (see subtitle_a.s).
@ Streams are rounded to four bytes; PACKER rows name the buffer's last writer.
	.ifeq EDITION_INTERNATIONAL

	.global Resource_SubtitleC
Resource_SubtitleC:
	.incbin "games/tla/src/graphics/title/subtitle_c_ja.8bpp64x64.plz"
	.byte 0x75, 0x08, 0x01	@ PACKER: left in the buffer by Resource_SubtitleB+0x815
	.global Resource_SubtitleD
Resource_SubtitleD:
	.incbin "games/tla/src/graphics/title/subtitle_d_ja.8bpp64x64.plz"

	.else
	.ifdef TLA_EDITION_DE

	.global Resource_SubtitleC
Resource_SubtitleC:
	.incbin "games/tla/src/graphics/title/subtitle_c_de.8bpp64x64.plz"
	.byte 0x49	@ PACKER: left in the buffer by Resource_SubtitleB+0x8f7
	.global Resource_SubtitleD
Resource_SubtitleD:
	.incbin "games/tla/src/graphics/title/subtitle_d_de.8bpp64x64.plz"

	.else
	.ifdef TLA_EDITION_ES

	.global Resource_SubtitleC
Resource_SubtitleC:
	.incbin "games/tla/src/graphics/title/subtitle_c_es.8bpp64x64.plz"
	.global Resource_SubtitleD
Resource_SubtitleD:
	.incbin "games/tla/src/graphics/title/subtitle_d_es.8bpp64x64.plz"
	.byte 0x03, 0x19, 0xcb	@ PACKER: left in the buffer by Resource_SubtitleC+0x6a1

	.else
	.ifdef TLA_EDITION_FR

	.global Resource_SubtitleC
Resource_SubtitleC:
	.incbin "games/tla/src/graphics/title/subtitle_c_fr.8bpp64x64.plz"
	.byte 0x08, 0x18	@ PACKER: left in the buffer by Resource_SubtitleB+0x7aa
	.global Resource_SubtitleD
Resource_SubtitleD:
	.incbin "games/tla/src/graphics/title/subtitle_d_fr.8bpp64x64.plz"
	.byte 0x14	@ PACKER: left in the buffer by Resource_SubtitleC+0x56b

	.else
	.ifdef TLA_EDITION_IT

	.global Resource_SubtitleC
Resource_SubtitleC:
	.incbin "games/tla/src/graphics/title/subtitle_c_it.8bpp64x64.plz"
	.byte 0xf9	@ PACKER: left in the buffer by Resource_SubtitleB+0x743
	.global Resource_SubtitleD
Resource_SubtitleD:
	.incbin "games/tla/src/graphics/title/subtitle_d_it.8bpp64x64.plz"

	.else

	.global Resource_SubtitleC
Resource_SubtitleC:
	.incbin "games/tla/src/graphics/title/subtitle_c.8bpp64x64.plz"
	.byte 0x02	@ PACKER: left in the buffer by Resource_SubtitleB+0x877
	.global Resource_SubtitleD
Resource_SubtitleD:
	.incbin "games/tla/src/graphics/title/subtitle_d.8bpp64x64.plz"
	.byte 0xd5, 0x06	@ PACKER: left in the buffer by Resource_SubtitleC+0x5d2

	.endif
	.endif
	.endif
	.endif
	.endif
