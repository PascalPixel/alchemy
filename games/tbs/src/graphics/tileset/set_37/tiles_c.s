	.section .rodata
	.global Tileset_Set37TilesC
Tileset_Set37TilesC:
	.incbin "graphics/tileset/set_37/tiles_c.4bpp.mtf"
	.byte 0x23, 0xe7	@ PACKER: left in the buffer by Tileset_Set36TilesB+0x27fe
