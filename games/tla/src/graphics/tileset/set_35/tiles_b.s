	.section .rodata
	.global Tileset_Set35TilesB
Tileset_Set35TilesB:
	.incbin "games/tla/src/graphics/tileset/set_35/tiles_b.4bpp.mtf"
	.byte 0xa9, 0x03	@ PACKER: left in the buffer by Tileset_Set35TilesA+0x1b56
