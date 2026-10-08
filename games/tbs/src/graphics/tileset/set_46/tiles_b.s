	.section .rodata
	.global Tileset_Set46TilesB
Tileset_Set46TilesB:
	.incbin "graphics/tileset/set_46/tiles_b.4bpp.mtf"
	.byte 0x10, 0xa1	@ PACKER: left in the buffer by Tileset_Set46TilesA+0x218e
