	.section .rodata
	.global Tileset_Set35TilesC
Tileset_Set35TilesC:
	.incbin "games/tla/src/graphics/tileset/set_35/tiles_c.4bpp.mtf"
	.byte 0xf5, 0xc0, 0x8e	@ PACKER: left in the buffer by Tileset_Set35TilesB+0xf45
