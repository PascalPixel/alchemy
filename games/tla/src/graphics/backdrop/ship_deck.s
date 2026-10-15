	.section .rodata
	.global Battle_ShipDeckBackdrop
Battle_ShipDeckBackdrop:
	.incbin "games/tla/src/graphics/backdrop/ship_deck.gbapal"
	.incbin "games/tla/src/graphics/backdrop/ship_deck.bitmap.d7"
	.byte 0x9d, 0x41	@ PACKER: left in the buffer by Battle_ShipCabinBackdrop+0x3a22
