	.section .rodata
	.global Slots_TimesSign
Slots_TimesSign:
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/times_sign_de.gbapal"
	.incbin "games/tla/src/graphics/slots/times_sign_de.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/times_sign_es.gbapal"
	.incbin "games/tla/src/graphics/slots/times_sign_es.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/times_sign_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/times_sign_fr.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/times_sign_it.gbapal"
	.incbin "games/tla/src/graphics/slots/times_sign_it.4bpp16x16.lz"
	.else
	.incbin "games/tla/src/graphics/slots/times_sign.gbapal"
	.incbin "games/tla/src/graphics/slots/times_sign.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x26	@ PACKER: left in the buffer by Slots_GreenDigits+0x10b
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x00, 0x81, 0xc2	@ PACKER: left in the buffer by Slots_GreenDigits+0xc9
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x45, 0x8a	@ PACKER: left in the buffer by Slots_GreenDigits+0xf2
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xd3	@ PACKER: left in the buffer by Slots_GreenDigits+0xbf
	.else
	.byte 0x91, 0x4c, 0x7b	@ PACKER: left in the buffer by Slots_GreenDigits+0xdd
	.endif
	.endif
	.endif
	.endif
