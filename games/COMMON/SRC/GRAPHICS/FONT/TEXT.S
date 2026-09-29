@ UiText_Glyphs: the text font, one record per character from 0x20: its
@ advance width, then its fifteen rows of sixteen pixels.
	.section .rodata
	.global UiText_Glyphs
UiText_Glyphs:
	.ifdef TBS_EDITION_DE
	.incbin "GRAPHICS/FONT/TEXT_DE.font"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/FONT/TEXT_IT.font"
	.else
	.incbin "GRAPHICS/FONT/TEXT.font"
	.endif
	.endif
