	.section .rodata
	.global Tileset_Set24TilesD
Tileset_Set24TilesD:
	.incbin "games/tla/src/graphics/tileset/set_24/tiles_d.4bpp.mtf"
	.byte 0x22, 0xb9	@ PACKER: left in the buffer by Tileset_Set24TilesC+0xf1e
