	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Title_CamelotLogo
Title_CamelotLogo:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/TITLE/CAMELOT_LOGO_JA.parts.lz"
	.else
	.incbin "GRAPHICS/TITLE/CAMELOT_LOGO.parts.lz"
	.endif
