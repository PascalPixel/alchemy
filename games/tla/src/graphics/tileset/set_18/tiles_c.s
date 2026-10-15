	.section .rodata
	.global Tileset_Set18TilesC
Tileset_Set18TilesC:
	.incbin "games/tla/src/graphics/tileset/set_18/tiles_c.4bpp.mtf"
	.byte 0x83, 0xb4, 0x9c	@ PACKER: left in the buffer by Tileset_Set18TilesB+0x1f5
