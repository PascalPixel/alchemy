	.section .rodata
	.global Tileset_Set54TilesB
Tileset_Set54TilesB:
	.incbin "games/tla/src/graphics/tileset/set_54/tiles_b.4bpp.mtf"
	.byte 0x08	@ PACKER: left in the buffer by Tileset_Set54TilesA+0x2033
