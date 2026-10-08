	.section .rodata
	.global Tileset_Set56TilesB
Tileset_Set56TilesB:
	.incbin "graphics/tileset/set_56/tiles_b.4bpp.mtf"
	.byte 0x79, 0x3a	@ PACKER: left in the buffer by Tileset_Set54TilesB+0x20fe
