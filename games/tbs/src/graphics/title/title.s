	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Title_IntroGraphicsA
Title_IntroGraphicsA:
	.incbin "games/tbs/src/graphics/title/intro_sun_1.gbapal"
	.incbin "games/tbs/src/graphics/title/intro_sun.parts.plz"
	.ifeq EDITION_INTERNATIONAL
	.byte 0x43	@ PACKER: left in the buffer by Ui_WindowTiles+0x1177
	.else
	.byte 0x44	@ PACKER: left in the buffer by Ui_WindowTiles+0x1177
	.endif
	.global Title_IntroGraphicsB
Title_IntroGraphicsB:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tbs/src/graphics/title/screen_ja.gbapal"
	.incbin "games/tbs/src/graphics/title/screen_ja.8bpp.plz"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "games/tbs/src/graphics/title/screen_de.gbapal"
	.incbin "games/tbs/src/graphics/title/screen_de.8bpp.plz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/title/screen_de.gbapal"
	.incbin "games/tbs/src/graphics/title/screen_de.8bpp.plz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/title/screen_de.gbapal"
	.incbin "games/tbs/src/graphics/title/screen_de.8bpp.plz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/title/screen_de.gbapal"
	.incbin "games/tbs/src/graphics/title/screen_de.8bpp.plz"
	.else
	.incbin "games/tbs/src/graphics/title/screen.gbapal"
	.incbin "games/tbs/src/graphics/title/screen.8bpp.plz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
	.include "games/common/src/graphics/title/intro_peak.s"
	.include "games/common/src/graphics/title/nintendo_splash.s"
	.global Title_CamelotLogo
Title_CamelotLogo:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tbs/src/graphics/title/camelot_logo_ja.parts.lz"
	.else
	.incbin "games/tbs/src/graphics/title/camelot_logo.parts.lz"
	.byte 0x04, 0x18, 0x03	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x1829
	.endif
	.global Title_GoldenSunLogo
Title_GoldenSunLogo:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tbs/src/graphics/title/golden_sun_ja.gbapal"
	.incbin "games/tbs/src/graphics/title/golden_sun_ja.8bpp.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/title/golden_sun_fr.gbapal"
	.incbin "games/tbs/src/graphics/title/golden_sun_fr.8bpp.lz"
	.byte 0x08, 0x01, 0x29	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x1921
	.else
	.incbin "games/tbs/src/graphics/title/golden_sun.gbapal"
	.incbin "games/tbs/src/graphics/title/golden_sun.8bpp.lz"
	.byte 0xd1, 0x27, 0x47	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x190d
	.endif
	.endif
	.global Field_WorldMapPicture
Field_WorldMapPicture:
	.incbin "games/tbs/src/graphics/world_map/picture.gbapal"
	.incbin "games/tbs/src/graphics/world_map/picture.8bpp.lz"
	.byte 0x02, 0x31, 0x01	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x3b01
@ 「つづく」の文字
	.global Title_IntroTilesB
Title_IntroTilesB:
	.ifdef TBS_EDITION_DE
	.incbin "games/tbs/src/graphics/title/tsuzuku_de.gbapal"
	.incbin "games/tbs/src/graphics/title/tsuzuku_de.8bpp.lz"
	.byte 0xab, 0x02	@ PACKER: left in the buffer by Field_WorldMapPicture+0x876
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/title/tsuzuku_es.gbapal"
	.incbin "games/tbs/src/graphics/title/tsuzuku_es.8bpp.lz"
	.byte 0xe8, 0x30, 0x22	@ PACKER: left in the buffer by Field_WorldMapPicture+0x719
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/title/tsuzuku_fr.gbapal"
	.incbin "games/tbs/src/graphics/title/tsuzuku_fr.8bpp.lz"
	.byte 0x85, 0x01, 0x5b	@ PACKER: left in the buffer by Field_WorldMapPicture+0x6a5
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/title/tsuzuku_it.gbapal"
	.incbin "games/tbs/src/graphics/title/tsuzuku_it.8bpp.lz"
	.byte 0x8a	@ PACKER: left in the buffer by Field_WorldMapPicture+0x687
	.else
	.incbin "games/common/src/graphics/title/tsuzuku.parts.lz"
	.endif
	.endif
	.endif
	.endif
