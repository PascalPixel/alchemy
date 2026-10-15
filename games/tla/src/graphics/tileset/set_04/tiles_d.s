	.section .rodata
	.global Tileset_Set04TilesD
Tileset_Set04TilesD:
	.incbin "games/tla/src/graphics/tileset/set_04/tiles_d.4bpp.mtf"
	.byte 0xf0	@ PACKER: left in the buffer by Tileset_Set04TilesC+0x13f
