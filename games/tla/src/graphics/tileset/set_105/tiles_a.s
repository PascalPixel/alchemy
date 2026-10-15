	.section .rodata
	.global Tileset_Set105TilesA
Tileset_Set105TilesA:
	.incbin "games/tla/src/graphics/tileset/set_105/tiles_a.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_Set103TilesB+0x25cf
