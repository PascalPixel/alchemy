	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Title_IntroGraphicsA
Title_IntroGraphicsA:
	.incbin "graphics/title/intro_sun_1.gbapal"
	.incbin "graphics/title/intro_sun.parts.plz"
	.ifeq EDITION_INTERNATIONAL
	.byte 0x43	@ PACKER: left in the buffer by Ui_WindowTiles+0x1177
	.else
	.byte 0x44	@ PACKER: left in the buffer by Ui_WindowTiles+0x1177
	.endif
