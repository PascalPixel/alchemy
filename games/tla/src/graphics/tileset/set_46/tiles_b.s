	.section .rodata
	.global Tileset_Set46TilesB
Tileset_Set46TilesB:
	.incbin "games/tla/src/graphics/tileset/set_46/tiles_b.4bpp.mtf"
	.byte 0xc8, 0x02, 0x58	@ PACKER: left in the buffer by Tileset_Set46TilesA+0x1dd1
