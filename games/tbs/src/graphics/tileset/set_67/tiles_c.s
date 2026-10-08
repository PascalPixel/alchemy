	.section .rodata
	.global Tileset_Set67TilesC
Tileset_Set67TilesC:
	.incbin "graphics/tileset/set_67/tiles_c.4bpp.mtf"
	.byte 0x0f, 0x7c, 0xfe	@ PACKER: left in the buffer by Tileset_Set67TilesB+0x2a5
