	.section .rodata
	.global Tileset_Set16TilesD
Tileset_Set16TilesD:
	.incbin "graphics/tileset/set_16/tiles_d.4bpp.mtf"
	.byte 0x82, 0xb0	@ PACKER: left in the buffer by Tileset_Set16TilesA+0xda2
