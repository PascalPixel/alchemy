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
	.ifdef TLA_EDITION_DE
	.byte 0x21	@ PACKER: left in the buffer by Resource_Data087+0x613
	.else
	.ifdef TLA_EDITION_ES
	.byte 0xa4, 0x62	@ PACKER: left in the buffer by Resource_Data087+0x61e
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x17	@ PACKER: left in the buffer by Resource_Data087+0x5e7
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xa1, 0x82	@ PACKER: left in the buffer by Resource_Data087+0x582
	.else
	.byte 0x78, 0x00, 0xd2	@ PACKER: left in the buffer by Resource_Data087+0x455
	.endif
	.endif
	.endif
	.endif
