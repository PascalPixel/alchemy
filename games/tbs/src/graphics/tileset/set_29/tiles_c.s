	.section .rodata
	.global Tileset_Set29TilesC
Tileset_Set29TilesC:
	.incbin "graphics/tileset/set_29/tiles_c.4bpp.mtf"
	.byte 0xba	@ PACKER: left in the buffer by Tileset_Set29TilesA+0x2847
