@ Battle_ShipDeckBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_ShipDeckBackdrop
Battle_ShipDeckBackdrop:
	.incbin "GRAPHICS/BACKDROP/SHIP_DECK.gbapal"
	.incbin "GRAPHICS/BACKDROP/SHIP_DECK.bitmap.d7"
