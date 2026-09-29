@ Slots_FinalGameBanner: its 16-colour palette, then 32 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_FinalGameBanner
Slots_FinalGameBanner:
	.incbin "GRAPHICS/SLOTS/FINAL_GAME.gbapal"
	.incbin "GRAPHICS/SLOTS/FINAL_GAME.4bpp16x16.lz"
