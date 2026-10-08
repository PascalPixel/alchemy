	.section .rodata
	.global Tileset_Set61TilesB
Tileset_Set61TilesB:
	.incbin "graphics/tileset/set_61/tiles_b.4bpp.mtf"
	.byte 0x6d	@ PACKER: left in the buffer by Tileset_Set61TilesA+0x1c4b
