	.section .rodata
	.global Tileset_Set14TilesD
Tileset_Set14TilesD:
	.incbin "games/tla/src/graphics/tileset/set_14/tiles_d.4bpp.mtf"
	.byte 0x08	@ PACKER: left in the buffer by Tileset_Set14TilesC+0x6c3
