	.section .rodata
	.global Tileset_Set70TilesD
Tileset_Set70TilesD:
	.incbin "graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set70TilesA+0x13f
