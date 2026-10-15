	.section .rodata
	.global Tileset_Set95TilesB
Tileset_Set95TilesB:
	.incbin "games/tla/src/graphics/tileset/set_95/tiles_b.4bpp.mtf"
	.byte 0x5b, 0x44, 0x03	@ PACKER: left in the buffer by Tileset_Set95TilesA+0x1679
