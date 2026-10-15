	.section .rodata
	.global Tileset_Set19TilesD
Tileset_Set19TilesD:
	.incbin "games/tla/src/graphics/tileset/set_19/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_Set19TilesC+0x1be
