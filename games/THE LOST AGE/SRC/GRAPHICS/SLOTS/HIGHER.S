@ Slots_HigherBanner: its 16-colour palette, then 48 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_HigherBanner
Slots_HigherBanner:
	.ifdef TLA_EDITION_JA
	.incbin "GRAPHICS/SLOTS/HIGHER_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/HIGHER_JA.4bpp32x32.lz"
	.else
	.incbin "GRAPHICS/SLOTS/HIGHER.gbapal"
	.incbin "GRAPHICS/SLOTS/HIGHER.4bpp32x32.lz"
	.endif
