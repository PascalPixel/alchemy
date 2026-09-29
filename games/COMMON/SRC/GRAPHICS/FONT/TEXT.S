@ UiText_Glyphs: the text font, one record per character from 0x20: its
@ advance width, then its rows of sixteen pixels (fifteen in the European
@ fonts, twelve in the Japanese ones, whose kanji from 0x100 are all one
@ width and store no width).
	.section .rodata
	.global UiText_Glyphs
UiText_Glyphs:
	.ifdef TBS_EDITION_JA
	.incbin "GRAPHICS/FONT/TEXT_JA.font"
	.else
	.ifdef TLA_EDITION_JA
	.incbin "GRAPHICS/FONT/TEXT_TLA_JA.font"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "GRAPHICS/FONT/TEXT_DE.font"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/FONT/TEXT_ES.font"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/FONT/TEXT_FR.font"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/FONT/TEXT_IT.font"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/FONT/TEXT_TLA_DE.font"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/FONT/TEXT_TLA_ES.font"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/FONT/TEXT_TLA_FR.font"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/FONT/TEXT_TLA_IT.font"
	.else
	.incbin "GRAPHICS/FONT/TEXT.font"
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
