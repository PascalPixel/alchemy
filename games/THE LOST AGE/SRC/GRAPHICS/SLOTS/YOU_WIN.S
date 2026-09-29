@ Slots_YouWinBanner: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_YouWinBanner
Slots_YouWinBanner:
	.incbin "GRAPHICS/SLOTS/YOU_WIN.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_WIN.4bpp.lz"
