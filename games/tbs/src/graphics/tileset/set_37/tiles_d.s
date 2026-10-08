	.section .rodata
	.global Tileset_Set37TilesD
Tileset_Set37TilesD:
	.incbin "graphics/tileset/set_37/tiles_d.4bpp.mtf"
	.byte 0x09	@ PACKER: left in the buffer by Tileset_Set37TilesC+0x13f
