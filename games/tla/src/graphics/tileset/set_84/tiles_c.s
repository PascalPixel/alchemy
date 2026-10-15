	.section .rodata
	.global Tileset_Set84TilesC
Tileset_Set84TilesC:
	.incbin "games/tla/src/graphics/tileset/set_84/tiles_c.4bpp.mtf"
	.byte 0x44, 0xd3	@ PACKER: left in the buffer by Tileset_Set84TilesB+0xb3a
