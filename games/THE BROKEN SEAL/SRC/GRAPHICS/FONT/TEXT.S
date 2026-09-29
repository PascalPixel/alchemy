@ UiText_Glyphs: the text font, one record per character from 0x20: its
@ advance width, then its fifteen rows of sixteen pixels.
	.section .rodata
	.global UiText_Glyphs
UiText_Glyphs:
	.incbin "GRAPHICS/FONT/TEXT.font"
