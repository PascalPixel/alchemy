	.section .rodata
	.global Tileset_Set66TilesC
Tileset_Set66TilesC:
	.incbin "games/tla/src/graphics/tileset/set_66/tiles_c.4bpp.mtf"
	.byte 0x50, 0x22	@ PACKER: left in the buffer by Tileset_Set66TilesA+0x10d6
