	.section .rodata
	.global Tileset_Set05TilesD
Tileset_Set05TilesD:
	.incbin "games/tla/src/graphics/tileset/set_05/tiles_d.4bpp.mtf"
	.byte 0x22, 0x60, 0x76	@ PACKER: left in the buffer by Tileset_Set05TilesC+0x8dd
