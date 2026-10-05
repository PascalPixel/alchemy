	.section .rodata
	.global Slots_OrangeDigits
Slots_OrangeDigits:
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/ORANGE_DIGITS_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/ORANGE_DIGITS_ES.4bpp16x32.lz"
	.else
	.incbin "GRAPHICS/SLOTS/ORANGE_DIGITS.gbapal"
	.incbin "GRAPHICS/SLOTS/ORANGE_DIGITS.4bpp16x32.lz"
	.endif
