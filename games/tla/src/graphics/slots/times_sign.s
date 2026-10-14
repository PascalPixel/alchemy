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
