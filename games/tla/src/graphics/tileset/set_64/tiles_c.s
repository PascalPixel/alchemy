	.section .rodata
	.global Tileset_Set64TilesC
Tileset_Set64TilesC:
	.incbin "games/tla/src/graphics/tileset/set_64/tiles_c.4bpp.mtf"
	.byte 0xf5	@ PACKER: left in the buffer by Tileset_Set64TilesB+0x697
