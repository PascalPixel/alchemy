	.section .rodata
	.global Battle_PinkRocksBackdrop
Battle_PinkRocksBackdrop:
	.incbin "games/tla/src/graphics/backdrop/pink_rocks.gbapal"
	.incbin "games/tla/src/graphics/backdrop/pink_rocks.bitmap.d7"
	.byte 0x45, 0x40	@ PACKER: left in the buffer by Battle_RedMesaBackdrop+0x39ee
