	.section .rodata
	.global Tileset_Set67TilesC
Tileset_Set67TilesC:
	.incbin "games/tla/src/graphics/tileset/set_67/tiles_c.4bpp.mtf"
	.byte 0x0f, 0x74, 0xf6	@ PACKER: left in the buffer by Tileset_Set67TilesB+0x1881
