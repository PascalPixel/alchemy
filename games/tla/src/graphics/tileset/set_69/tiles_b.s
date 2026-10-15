	.section .rodata
	.global Tileset_Set69TilesB
Tileset_Set69TilesB:
	.incbin "games/tla/src/graphics/tileset/set_69/tiles_b.4bpp.mtf"
	.byte 0xa8, 0x53	@ PACKER: left in the buffer by Tileset_Set69TilesA+0x112a
