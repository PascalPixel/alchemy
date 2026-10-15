	.section .rodata
	.global Tileset_Set78TilesB
Tileset_Set78TilesB:
	.incbin "games/tla/src/graphics/tileset/set_78/tiles_b.4bpp.mtf"
	.byte 0xa2	@ PACKER: left in the buffer by Tileset_Set78TilesA+0x140b
