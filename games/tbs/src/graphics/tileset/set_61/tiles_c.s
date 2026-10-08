	.section .rodata
	.global Tileset_Set61TilesC
Tileset_Set61TilesC:
	.incbin "graphics/tileset/set_61/tiles_c.4bpp.mtf"
	.byte 0x06, 0x21, 0x87	@ PACKER: left in the buffer by Tileset_Set61TilesB+0x1445
