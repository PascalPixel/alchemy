@ UiText_SmallFont: 224 glyphs of 8x10 for codes 0x20 to 0xff, one bit a
@ pixel, a byte a row.
	.section .rodata
	.global UiText_SmallFont
UiText_SmallFont:
	.incbin "GRAPHICS/UI/SMALL_FONT.glyphs"
