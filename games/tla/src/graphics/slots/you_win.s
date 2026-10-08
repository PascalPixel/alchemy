	.section .rodata
	.global Slots_YouWinBanner
Slots_YouWinBanner:
	.ifdef TLA_EDITION_DE
	.incbin "graphics/slots/you_win_de.gbapal"
	.incbin "graphics/slots/you_win_de.4bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/you_win_es.gbapal"
	.incbin "graphics/slots/you_win_es.4bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/slots/you_win_fr.gbapal"
	.incbin "graphics/slots/you_win_fr.4bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/slots/you_win_it.gbapal"
	.incbin "graphics/slots/you_win_it.4bpp.lz"
	.else
	.incbin "graphics/slots/you_win.gbapal"
	.incbin "graphics/slots/you_win.4bpp.lz"
	.endif
	.endif
	.endif
	.endif
