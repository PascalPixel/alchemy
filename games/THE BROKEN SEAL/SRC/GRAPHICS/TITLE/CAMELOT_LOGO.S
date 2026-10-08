	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Title_CamelotLogo
Title_CamelotLogo:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/TITLE/CAMELOT_LOGO_JA.parts.lz"
	.else
	.incbin "GRAPHICS/TITLE/CAMELOT_LOGO.parts.lz"
	/* PACKER: left in the packer's buffer by Title_IntroGraphicsC at +0x1829 */
	.byte 0x04, 0x18, 0x03
	.endif
