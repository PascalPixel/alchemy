	.section .rodata
	.global Tileset_Set66TilesD
Tileset_Set66TilesD:
	.incbin "graphics/tileset/set_66/tiles_d.4bpp.mtf"
	.byte 0x04, 0x5c	@ PACKER: left in the buffer by Tileset_Set66TilesC+0x5b2
