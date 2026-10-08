	.section .rodata
	.global Tileset_Set60TilesD
Tileset_Set60TilesD:
	.incbin "graphics/tileset/set_60/tiles_d.4bpp.mtf"
	.byte 0x69	@ PACKER: left in the buffer by Tileset_Set60TilesB+0x1907
