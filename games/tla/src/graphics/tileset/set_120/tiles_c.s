	.section .rodata
	.global Tileset_Set120TilesC
Tileset_Set120TilesC:
	.incbin "games/tla/src/graphics/tileset/set_120/tiles_c.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_Set120TilesB+0x13f
