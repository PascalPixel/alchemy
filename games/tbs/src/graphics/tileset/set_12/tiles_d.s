	.section .rodata
	.global Tileset_Set12TilesD
Tileset_Set12TilesD:
	.incbin "graphics/tileset/set_12/tiles_d.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Tileset_Set12TilesC+0x13f
