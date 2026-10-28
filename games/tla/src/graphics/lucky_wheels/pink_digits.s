	.section .rodata
	.global Slots_PinkDigits
Slots_PinkDigits:
	.incbin "games/tla/src/graphics/lucky_wheels/pink_digits.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/pink_digits.4bpp16x16.lz"
	.ifdef TLA_EDITION_EN
	.byte 0xd9, 0x20, 0xbe	@ PACKER: left in the buffer by Slots_CashOutBanner+0x36d
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x97, 0x23, 0x59	@ PACKER: left in the buffer by Slots_CashOutBanner+0x36d
	.else
	.ifdef TLA_EDITION_ES
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x19, 0x07, 0x38	@ PACKER: left in the buffer by Slots_CashOutBanner+0x36d
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x00, 0x0c, 0xe0	@ PACKER: left in the buffer by Slots_CashOutBanner+0x36d
	.else
	.byte 0xae, 0xf0, 0x1c	@ PACKER: left in the buffer by Slots_DoubleUpBanner+0x36d
	.endif
	.endif
	.endif
	.endif
	.endif
