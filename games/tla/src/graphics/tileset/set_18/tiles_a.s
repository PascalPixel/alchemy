	.section .rodata
	.global Tileset_Set18TilesA
Tileset_Set18TilesA:
	.incbin "games/tla/src/graphics/tileset/set_18/tiles_a.4bpp.mtf"
	.byte 0xf8, 0x3e	@ PACKER: left in the buffer by Tileset_Set17TilesC+0x1b56
