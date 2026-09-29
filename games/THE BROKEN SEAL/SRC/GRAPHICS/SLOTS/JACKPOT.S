@ Slots_Jackpot: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_Jackpot
Slots_Jackpot:
	.incbin "GRAPHICS/SLOTS/JACKPOT.gbapal"
	.incbin "GRAPHICS/SLOTS/JACKPOT.4bpp.lz"
