	.section .rodata
	.global Tileset_Set45TilesD
Tileset_Set45TilesD:
	.incbin "graphics/tileset/set_45/tiles_d.4bpp.mtf"
	.byte 0xfb, 0x14, 0x4d	@ PACKER: left in the buffer by Tileset_Set45TilesC+0xb99
