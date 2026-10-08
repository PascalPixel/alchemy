	.section .rodata
	.global Tileset_Set21TilesC
Tileset_Set21TilesC:
	.incbin "graphics/tileset/set_21/tiles_c.4bpp.mtf"
	.byte 0x83, 0x38	@ PACKER: left in the buffer by Tileset_Set21TilesB+0x2056
