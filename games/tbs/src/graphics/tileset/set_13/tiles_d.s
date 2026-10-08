	.section .rodata
	.global Tileset_Set13TilesD
Tileset_Set13TilesD:
	.incbin "graphics/tileset/set_13/tiles_d.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Tileset_Set13TilesC+0x13f
