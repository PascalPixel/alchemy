	.section .rodata
	.global Tileset_Set41TilesC
Tileset_Set41TilesC:
	.incbin "graphics/tileset/set_41/tiles_c.4bpp.mtf"
	.byte 0xde	@ PACKER: left in the buffer by Tileset_Set40TilesA+0x2633
