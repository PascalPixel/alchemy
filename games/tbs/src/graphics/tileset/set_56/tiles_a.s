	.section .rodata
	.global Tileset_Set56TilesA
Tileset_Set56TilesA:
	.incbin "graphics/tileset/set_56/tiles_a.4bpp.mtf"
	.byte 0xd1, 0x43	@ PACKER: left in the buffer by Tileset_Set54TilesB+0x208e
