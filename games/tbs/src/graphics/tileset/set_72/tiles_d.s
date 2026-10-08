	.section .rodata
	.global Tileset_Set72TilesD
Tileset_Set72TilesD:
	.incbin "graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set72TilesA+0x13f
