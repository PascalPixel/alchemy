	.section .rodata
	.global Tileset_Set91TilesA
Tileset_Set91TilesA:
	.incbin "games/tla/src/graphics/tileset/set_91/tiles_a.4bpp.mtf"
	.byte 0xb8	@ PACKER: left in the buffer by Tileset_Set90TilesC+0x2677
