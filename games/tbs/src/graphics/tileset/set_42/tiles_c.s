	.section .rodata
	.global Tileset_Set42TilesC
Tileset_Set42TilesC:
	.incbin "graphics/tileset/set_42/tiles_c.4bpp.mtf"
	.byte 0xc3, 0x73, 0x89	@ PACKER: left in the buffer by Tileset_Set42TilesB+0x1c59
