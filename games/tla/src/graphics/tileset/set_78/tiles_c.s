	.section .rodata
	.global Tileset_Set78TilesC
Tileset_Set78TilesC:
	.incbin "games/tla/src/graphics/tileset/set_78/tiles_c.4bpp.mtf"
	.byte 0x02	@ PACKER: left in the buffer by Tileset_Set78TilesB+0x553
