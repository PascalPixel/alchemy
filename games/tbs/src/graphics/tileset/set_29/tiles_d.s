	.section .rodata
	.global Tileset_Set29TilesD
Tileset_Set29TilesD:
	.incbin "graphics/tileset/set_29/tiles_d.4bpp.mtf"
	.byte 0x68, 0x18, 0x2a	@ PACKER: left in the buffer by Tileset_Set29TilesC+0x509
