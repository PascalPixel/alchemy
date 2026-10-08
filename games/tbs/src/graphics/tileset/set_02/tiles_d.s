	.section .rodata
	.global Tileset_Set02TilesD
Tileset_Set02TilesD:
	.incbin "graphics/tileset/set_02/tiles_d.4bpp.mtf"
	.byte 0xf7	@ PACKER: left in the buffer by Tileset_Set02TilesC+0x1073
