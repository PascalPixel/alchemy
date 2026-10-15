	.section .rodata
	.global Tileset_Set23TilesD
Tileset_Set23TilesD:
	.incbin "games/tla/src/graphics/tileset/set_23/tiles_d.4bpp.mtf"
	.byte 0x0c, 0xbc	@ PACKER: left in the buffer by Tileset_Set23TilesC+0x15e6
