@ Slots_YouLoseBanner: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_YouLoseBanner
Slots_YouLoseBanner:
	.incbin "GRAPHICS/SLOTS/YOU_LOSE.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_LOSE.4bpp.lz"
