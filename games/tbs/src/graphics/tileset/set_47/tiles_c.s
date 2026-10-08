	.section .rodata
	.global Tileset_Set47TilesC
Tileset_Set47TilesC:
	.incbin "graphics/tileset/set_47/tiles_c.4bpp.mtf"
	.byte 0x2e, 0x1f	@ PACKER: left in the buffer by Tileset_Set47TilesB+0x1092
