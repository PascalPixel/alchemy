	.section .rodata
	.global Tileset_Set16TilesB
Tileset_Set16TilesB:
	.incbin "games/tla/src/graphics/tileset/set_16/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_Set16TilesA+0xfda
