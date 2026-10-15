	.section .rodata
	.global Tileset_Set119TilesA
Tileset_Set119TilesA:
	.incbin "games/tla/src/graphics/tileset/set_119/tiles_a.4bpp.mtf"
	.byte 0x5a, 0x33, 0xa4	@ PACKER: left in the buffer by Tileset_Set117TilesA+0x2651
