	.section .rodata
	.global Slots_YouWinBanner
Slots_YouWinBanner:
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/you_win_de.gbapal"
	.incbin "games/tla/src/graphics/slots/you_win_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/you_win_es.gbapal"
	.incbin "games/tla/src/graphics/slots/you_win_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/you_win_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/you_win_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/you_win_it.gbapal"
	.incbin "games/tla/src/graphics/slots/you_win_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/slots/you_win.gbapal"
	.incbin "games/tla/src/graphics/slots/you_win.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
