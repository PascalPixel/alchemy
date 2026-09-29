@ Slots_TimesSign: its 16-colour palette, then 12 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_TimesSign
Slots_TimesSign:
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN.gbapal"
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN.4bpp.lz"
