	.section .rodata
	.global Tileset_Set03TilesB
Tileset_Set03TilesB:
	.incbin "games/tla/src/graphics/tileset/set_03/tiles_b.4bpp.mtf"
	.byte 0x73	@ PACKER: left in the buffer by Tileset_Set03TilesA+0x28a7
