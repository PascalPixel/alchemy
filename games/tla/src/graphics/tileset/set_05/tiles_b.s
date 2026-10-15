	.section .rodata
	.global Tileset_Set05TilesB
Tileset_Set05TilesB:
	.incbin "games/tla/src/graphics/tileset/set_05/tiles_b.4bpp.mtf"
	.byte 0x04, 0x92, 0x9e	@ PACKER: left in the buffer by Tileset_Set04TilesC+0x1c7d
