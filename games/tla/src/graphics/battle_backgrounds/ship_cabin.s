	.section .rodata
	.global Battle_ShipCabinBackdrop
Battle_ShipCabinBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/ship_cabin.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/ship_cabin.bitmap.d7"
	.byte 0x03, 0x5c	@ PACKER: left in the buffer by Battle_PalmCliffBackdrop+0x3f2e
