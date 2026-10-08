	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Title_CamelotLogo
Title_CamelotLogo:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/TITLE/CAMELOT_LOGO_JA.parts.lz"
	.else
	.incbin "GRAPHICS/TITLE/CAMELOT_LOGO.parts.lz"
	.byte 0x04, 0x18, 0x03	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x1829
	.endif
