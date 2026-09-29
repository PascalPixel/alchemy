@ Slots_TripleBanner: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_TripleBanner
Slots_TripleBanner:
	.ifdef TLA_EDITION_JA
	.incbin "GRAPHICS/SLOTS/TRIPLE_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/TRIPLE_JA.4bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/TRIPLE_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/TRIPLE_DE.4bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/TRIPLE_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/TRIPLE_ES.4bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/TRIPLE_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/TRIPLE_FR.4bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/TRIPLE_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/TRIPLE_IT.4bpp.lz"
	.else
	.incbin "GRAPHICS/SLOTS/TRIPLE.gbapal"
	.incbin "GRAPHICS/SLOTS/TRIPLE.4bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
