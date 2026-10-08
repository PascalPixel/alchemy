	.section .rodata
	.global Tileset_Set31TilesB
Tileset_Set31TilesB:
	.incbin "graphics/tileset/set_31/tiles_b.4bpp.mtf"
	.byte 0x4d, 0x0c, 0x83	@ PACKER: left in the buffer by Tileset_Set31TilesA+0x1d6d
