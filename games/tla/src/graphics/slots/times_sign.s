	.section .rodata
	.global Slots_TimesSign
Slots_TimesSign:
	.ifdef TLA_EDITION_DE
	.incbin "graphics/slots/times_sign_de.gbapal"
	.incbin "graphics/slots/times_sign_de.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/times_sign_es.gbapal"
	.incbin "graphics/slots/times_sign_es.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/slots/times_sign_fr.gbapal"
	.incbin "graphics/slots/times_sign_fr.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/slots/times_sign_it.gbapal"
	.incbin "graphics/slots/times_sign_it.4bpp16x16.lz"
	.else
	.incbin "graphics/slots/times_sign.gbapal"
	.incbin "graphics/slots/times_sign.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
