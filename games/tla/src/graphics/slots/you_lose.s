	.section .rodata
	.global Slots_YouLoseBanner
Slots_YouLoseBanner:
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/you_lose_de.gbapal"
	.incbin "games/tla/src/graphics/slots/you_lose_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/you_lose_es.gbapal"
	.incbin "games/tla/src/graphics/slots/you_lose_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/you_lose_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/you_lose_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/you_lose_it.gbapal"
	.incbin "games/tla/src/graphics/slots/you_lose_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/slots/you_lose.gbapal"
	.incbin "games/tla/src/graphics/slots/you_lose.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x10	@ PACKER: left in the buffer by Slots_YouWinBanner+0x41f
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x90	@ PACKER: left in the buffer by Slots_YouWinBanner+0x577
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xcd, 0xe4, 0x07	@ PACKER: left in the buffer by Slots_YouWinBanner+0x4d5
	.endif
	.endif
	.endif
