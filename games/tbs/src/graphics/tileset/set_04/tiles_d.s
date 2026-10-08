	.section .rodata
	.global Tileset_Set04TilesD
Tileset_Set04TilesD:
	.incbin "graphics/tileset/set_04/tiles_d.4bpp.mtf"
	.byte 0x3c	@ PACKER: left in the buffer by Tileset_Set04TilesC+0x12c7
