@ Slots_LowerBanner: its 16-colour palette, then 48 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_LowerBanner
Slots_LowerBanner:
	.ifdef TLA_EDITION_JA
	.incbin "GRAPHICS/SLOTS/LOWER_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/LOWER_JA.4bpp32x32.lz"
	.else
	.incbin "GRAPHICS/SLOTS/LOWER.gbapal"
	.incbin "GRAPHICS/SLOTS/LOWER.4bpp32x32.lz"
	.endif
