	.section .rodata
	.global Tileset_Set75TilesD
Tileset_Set75TilesD:
	.incbin "graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set75TilesA+0x13f
