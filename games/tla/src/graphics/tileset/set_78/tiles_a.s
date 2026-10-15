	.section .rodata
	.global Tileset_Set78TilesA
Tileset_Set78TilesA:
	.incbin "games/tla/src/graphics/tileset/set_78/tiles_a.4bpp.mtf"
	.byte 0x1f	@ PACKER: left in the buffer by Tileset_Set77TilesA+0x1af7
