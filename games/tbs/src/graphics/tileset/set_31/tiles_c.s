	.section .rodata
	.global Tileset_Set31TilesC
Tileset_Set31TilesC:
	.incbin "graphics/tileset/set_31/tiles_c.4bpp.mtf"
	.byte 0xfa, 0x80	@ PACKER: left in the buffer by Tileset_Set31TilesB+0x1042
