	.section .rodata
	.global Tileset_Set54TilesD
Tileset_Set54TilesD:
	.incbin "games/tla/src/graphics/tileset/set_54/tiles_d.4bpp.mtf"
	.byte 0xe0, 0x7c	@ PACKER: left in the buffer by Tileset_Set54TilesC+0xbae
