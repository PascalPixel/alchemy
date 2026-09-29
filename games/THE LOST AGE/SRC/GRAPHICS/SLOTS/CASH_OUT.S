@ Slots_CashOutBanner: its 16-colour palette, then 80 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_CashOutBanner
Slots_CashOutBanner:
	.incbin "GRAPHICS/SLOTS/CASH_OUT.gbapal"
	.incbin "GRAPHICS/SLOTS/CASH_OUT.4bpp.lz"
