	.section .rodata
	.global Tileset_Set96TilesB
Tileset_Set96TilesB:
	.incbin "games/tla/src/graphics/tileset/set_96/tiles_b.4bpp.mtf"
	.byte 0xf2, 0x8b, 0xdd	@ PACKER: left in the buffer by Tileset_Set96TilesA+0x1365
