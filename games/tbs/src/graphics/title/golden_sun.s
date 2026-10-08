	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Title_GoldenSunLogo
Title_GoldenSunLogo:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/title/golden_sun_ja.gbapal"
	.incbin "graphics/title/golden_sun_ja.8bpp.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "graphics/title/golden_sun_fr.gbapal"
	.incbin "graphics/title/golden_sun_fr.8bpp.lz"
	.byte 0x08, 0x01, 0x29	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x1921
	.else
	.incbin "graphics/title/golden_sun.gbapal"
	.incbin "graphics/title/golden_sun.8bpp.lz"
	.byte 0xd1, 0x27, 0x47	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x190d
	.endif
	.endif
