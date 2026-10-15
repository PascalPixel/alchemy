	.section .rodata
	.global Tileset_Set13TilesD
Tileset_Set13TilesD:
	.incbin "games/tla/src/graphics/tileset/set_13/tiles_d.4bpp.mtf"
	.byte 0xc1, 0x01, 0xb0	@ PACKER: left in the buffer by Tileset_Set13TilesC+0x18fd
