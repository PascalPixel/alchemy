	.section .rodata
	.global Tileset_Set19TilesA
Tileset_Set19TilesA:
	.incbin "games/tla/src/graphics/tileset/set_19/tiles_a.4bpp.mtf"
	.byte 0xf8, 0x3e	@ PACKER: left in the buffer by Tileset_Set18TilesA+0x1b56
