	.section .rodata
	.global Tileset_Set58TilesC
Tileset_Set58TilesC:
	.incbin "graphics/tileset/set_58/tiles_c.4bpp.mtf"
	.byte 0xfe	@ PACKER: left in the buffer by Tileset_Set58TilesB+0x153
