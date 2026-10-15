	.section .rodata
	.global Tileset_Set118TilesC
Tileset_Set118TilesC:
	.incbin "games/tla/src/graphics/tileset/set_118/tiles_c.4bpp.mtf"
	.byte 0x56	@ PACKER: left in the buffer by Tileset_Set118TilesB+0x111b
