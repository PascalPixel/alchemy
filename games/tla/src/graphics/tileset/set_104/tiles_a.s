	.section .rodata
	.global Tileset_Set104TilesA
Tileset_Set104TilesA:
	.incbin "games/tla/src/graphics/tileset/set_104/tiles_a.4bpp.mtf"
	.byte 0xf1, 0x6c, 0x88	@ PACKER: left in the buffer by Tileset_Set103TilesC+0x1f3d
