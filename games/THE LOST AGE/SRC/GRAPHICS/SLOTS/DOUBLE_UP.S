@ Slots_DoubleUpBanner: its 16-colour palette, then 80 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_DoubleUpBanner
Slots_DoubleUpBanner:
	.ifdef TLA_EDITION_JA
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP_JA.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP_DE.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP_ES.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP_FR.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP_IT.4bpp32x32.lz"
	.else
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP.gbapal"
	.incbin "GRAPHICS/SLOTS/DOUBLE_UP.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
