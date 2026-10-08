	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Title_CamelotLogo
Title_CamelotLogo:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/title/camelot_logo_ja.parts.lz"
	.else
	.incbin "graphics/title/camelot_logo.parts.lz"
	.byte 0x04, 0x18, 0x03	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x1829
	.endif
