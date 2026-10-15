	.section .rodata
	.global Tileset_Set70TilesC
Tileset_Set70TilesC:
	.incbin "games/tla/src/graphics/tileset/set_70/tiles_c.4bpp.mtf"
	.byte 0x67	@ PACKER: left in the buffer by Tileset_Set70TilesB+0x13f
