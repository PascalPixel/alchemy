@ Battle_ShipCabinBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_ShipCabinBackdrop
Battle_ShipCabinBackdrop:
	.incbin "GRAPHICS/BACKDROP/SHIP_CABIN.gbapal"
	.incbin "GRAPHICS/BACKDROP/SHIP_CABIN.bitmap.d7"
