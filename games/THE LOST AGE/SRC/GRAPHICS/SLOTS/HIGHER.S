@ Slots_HigherBanner: its 16-colour palette, then 48 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_HigherBanner
Slots_HigherBanner:
	.ifdef TLA_EDITION_JA
	.incbin "GRAPHICS/SLOTS/HIGHER_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/HIGHER_JA.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/HIGHER_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/HIGHER_DE.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/HIGHER_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/HIGHER_ES.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/HIGHER_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/HIGHER_FR.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/HIGHER_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/HIGHER_IT.4bpp32x32.lz"
	.else
	.incbin "GRAPHICS/SLOTS/HIGHER.gbapal"
	.incbin "GRAPHICS/SLOTS/HIGHER.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
