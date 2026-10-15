	.section .rodata
	.global Tileset_Set112TilesC
Tileset_Set112TilesC:
	.incbin "games/tla/src/graphics/tileset/set_112/tiles_c.4bpp.mtf"
	.byte 0x10, 0x9e	@ PACKER: left in the buffer by Tileset_Set112TilesB+0x726
