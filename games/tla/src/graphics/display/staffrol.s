	.section .rodata
@ The staff credits font, characters 0x20 to 0x7f, 8x8 and 1 bit:
@ The Broken Seal's DisplayScroll_Font. The Spanish, French and Italian
@ fonts draw an i with an acute accent at '`', and the French and Italian
@ ones an accented capital A at 0x7f.
	.global DisplayScroll_Font
DisplayScroll_Font:
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/display/staffrol_es.1bpp"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/display/staffrol_fr.1bpp"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/display/staffrol_it.1bpp"
	.else
	.incbin "games/common/src/graphics/display/staffrol.1bpp"
	.endif
	.endif
	.endif
