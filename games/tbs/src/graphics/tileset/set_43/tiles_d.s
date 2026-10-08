	.section .rodata
	.global Tileset_Set43TilesD
Tileset_Set43TilesD:
	.incbin "graphics/tileset/set_43/tiles_d.4bpp.mtf"
	.byte 0x0c	@ PACKER: left in the buffer by Tileset_Set43TilesB+0x1943
