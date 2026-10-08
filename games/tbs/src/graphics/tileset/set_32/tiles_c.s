	.section .rodata
	.global Tileset_Set32TilesC
Tileset_Set32TilesC:
	.incbin "graphics/tileset/set_32/tiles_c.4bpp.mtf"
	.byte 0xb8, 0x92, 0x64	@ PACKER: left in the buffer by Tileset_Set32TilesB+0xa65
