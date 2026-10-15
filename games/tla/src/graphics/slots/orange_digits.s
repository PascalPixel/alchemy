	.section .rodata
	.global Slots_OrangeDigits
Slots_OrangeDigits:
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/orange_digits_es.gbapal"
	.incbin "games/tla/src/graphics/slots/orange_digits_es.4bpp16x32.lz"
	.else
	.incbin "games/tla/src/graphics/slots/orange_digits.gbapal"
	.incbin "games/tla/src/graphics/slots/orange_digits.4bpp16x32.lz"
	.endif
	.ifndef TLA_EDITION_JA
	.ifndef TLA_EDITION_EN
	.ifndef TLA_EDITION_DE
	.ifndef TLA_EDITION_FR
	.ifndef TLA_EDITION_IT
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
	.endif
	.endif
	.endif
