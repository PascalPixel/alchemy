	.section .rodata
	.global Tileset_Set62TilesD
Tileset_Set62TilesD:
	.incbin "games/tla/src/graphics/tileset/set_62/tiles_d.4bpp.mtf"
	.byte 0xa1, 0x4f, 0x64	@ PACKER: left in the buffer by Tileset_Set62TilesC+0x1445
