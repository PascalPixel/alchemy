@ Title_ScreenPicture: its 224-colour palette, then 600 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Title_ScreenPicture
Title_ScreenPicture:
	.ifdef TLA_EDITION_JA
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
