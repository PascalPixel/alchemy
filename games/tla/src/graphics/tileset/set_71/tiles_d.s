	.section .rodata
	.global Tileset_Set71TilesD
Tileset_Set71TilesD:
	.incbin "games/tla/src/graphics/tileset/set_71/tiles_d.4bpp.mtf"
	.byte 0x8c, 0x03	@ PACKER: left in the buffer by Tileset_Set71TilesB+0x1572
