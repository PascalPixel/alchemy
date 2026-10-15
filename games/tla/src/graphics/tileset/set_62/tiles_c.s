	.section .rodata
	.global Tileset_Set62TilesC
Tileset_Set62TilesC:
	.incbin "games/tla/src/graphics/tileset/set_62/tiles_c.4bpp.mtf"
	.byte 0x00, 0x5c	@ PACKER: left in the buffer by Tileset_Set62TilesB+0x15e2
