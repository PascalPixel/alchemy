	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Title_IntroGraphicsB: the title screen, its 256-colour palette, then 600
@ tiles of 8-bit colour in the byte LZ the resource loader decodes.
	.section .rodata
	.global Title_IntroGraphicsB
Title_IntroGraphicsB:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/TITLE/SCREEN_JA.gbapal"
	.incbin "GRAPHICS/TITLE/SCREEN_JA.8bpp.plz"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "GRAPHICS/TITLE/SCREEN_DE.gbapal"
	.incbin "GRAPHICS/TITLE/SCREEN_DE.8bpp.plz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/TITLE/SCREEN_DE.gbapal"
	.incbin "GRAPHICS/TITLE/SCREEN_DE.8bpp.plz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/TITLE/SCREEN_DE.gbapal"
	.incbin "GRAPHICS/TITLE/SCREEN_DE.8bpp.plz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/TITLE/SCREEN_DE.gbapal"
	.incbin "GRAPHICS/TITLE/SCREEN_DE.8bpp.plz"
	.else
	.incbin "GRAPHICS/TITLE/SCREEN.gbapal"
	.incbin "GRAPHICS/TITLE/SCREEN.8bpp.plz"
	.endif
	.endif
	.endif
	.endif
	.endif
