@ Title_LogoOnBlackPicture: its 224-colour palette, then 600 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Title_LogoOnBlackPicture
Title_LogoOnBlackPicture:
	.ifdef TLA_EDITION_JA
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK_JA.gbapal"
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK_JA.8bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK_DE.gbapal"
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK_DE.8bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK_ES.gbapal"
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK_ES.8bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK_FR.gbapal"
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK_FR.8bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK_IT.gbapal"
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK_IT.8bpp.lz"
	.else
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK.gbapal"
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK.8bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
