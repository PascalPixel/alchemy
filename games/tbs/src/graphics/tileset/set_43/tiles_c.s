	.section .rodata
	.global Tileset_Set43TilesC
Tileset_Set43TilesC:
	.incbin "graphics/tileset/set_43/tiles_c.4bpp.mtf"
	.byte 0x7a, 0x40, 0xe2	@ PACKER: left in the buffer by Tileset_Set43TilesB+0x12f9
