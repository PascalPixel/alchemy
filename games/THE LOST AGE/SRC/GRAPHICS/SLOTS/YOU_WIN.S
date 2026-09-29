@ Slots_YouWinBanner: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_YouWinBanner
Slots_YouWinBanner:
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/YOU_WIN_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_WIN_DE.4bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/YOU_WIN_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_WIN_ES.4bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/YOU_WIN_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_WIN_FR.4bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/YOU_WIN_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_WIN_IT.4bpp.lz"
	.else
	.incbin "GRAPHICS/SLOTS/YOU_WIN.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_WIN.4bpp.lz"
	.endif
	.endif
	.endif
	.endif
