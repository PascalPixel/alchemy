	.section .rodata
	.global Tileset_Set77TilesD
Tileset_Set77TilesD:
	.incbin "games/tla/src/graphics/tileset/set_77/tiles_d.4bpp.mtf"
	.byte 0x23, 0x70, 0x22	@ PACKER: left in the buffer by Tileset_Set77TilesC+0x639
