	.section .rodata
	.global Tileset_Set00TilesB
Tileset_Set00TilesB:
	.incbin "graphics/tileset/set_00/tiles_b.4bpp.mtf"
	.byte 0x5d, 0x04, 0x53	@ PACKER: left in the buffer by Tileset_Set00TilesA+0x2529
