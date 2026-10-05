	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Title_ScreenPicture
Title_ScreenPicture:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN_JA.gbapal"
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN_JA.8bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN_DE.gbapal"
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN_DE.8bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN_ES.gbapal"
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN_ES.8bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN_FR.gbapal"
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN_FR.8bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN_IT.gbapal"
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN_IT.8bpp.lz"
	.else
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN.gbapal"
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN.8bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
