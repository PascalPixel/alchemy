@ Slots_CashOutBanner: its 16-colour palette, then 80 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_CashOutBanner
Slots_CashOutBanner:
	.ifdef TLA_EDITION_JA
	.incbin "GRAPHICS/SLOTS/CASH_OUT_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/CASH_OUT_JA.4bpp32x32.lz"
	.else
	.incbin "GRAPHICS/SLOTS/CASH_OUT.gbapal"
	.incbin "GRAPHICS/SLOTS/CASH_OUT.4bpp32x32.lz"
	.endif
