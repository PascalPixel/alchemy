	.section .rodata
	.global Slots_YouLoseBanner
Slots_YouLoseBanner:
	.ifdef TLA_EDITION_DE
	.incbin "graphics/slots/you_lose_de.gbapal"
	.incbin "graphics/slots/you_lose_de.4bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/you_lose_es.gbapal"
	.incbin "graphics/slots/you_lose_es.4bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/slots/you_lose_fr.gbapal"
	.incbin "graphics/slots/you_lose_fr.4bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/slots/you_lose_it.gbapal"
	.incbin "graphics/slots/you_lose_it.4bpp.lz"
	.else
	.incbin "graphics/slots/you_lose.gbapal"
	.incbin "graphics/slots/you_lose.4bpp.lz"
	.endif
	.endif
	.endif
	.endif
