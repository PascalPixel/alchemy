	.section .rodata
	.global Tileset_Set37TilesB
Tileset_Set37TilesB:
	.incbin "graphics/tileset/set_37/tiles_b.4bpp.mtf"
	.byte 0x2f	@ PACKER: left in the buffer by Tileset_Set37TilesA+0x13f
