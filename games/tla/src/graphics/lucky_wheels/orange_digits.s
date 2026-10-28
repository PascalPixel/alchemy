	.section .rodata
	.global Slots_OrangeDigits
Slots_OrangeDigits:
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/orange_digits_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/orange_digits_es.4bpp16x32.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/orange_digits.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/orange_digits.4bpp16x32.lz"
	.endif
	.ifdef TLA_EDITION_ES
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
