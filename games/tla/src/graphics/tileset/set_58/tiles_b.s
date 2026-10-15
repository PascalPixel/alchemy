	.section .rodata
	.global Tileset_Set58TilesB
Tileset_Set58TilesB:
	.incbin "games/tla/src/graphics/tileset/set_58/tiles_b.4bpp.mtf"
	.byte 0x4d, 0xcb, 0x82	@ PACKER: left in the buffer by Tileset_Set57TilesA+0x1479
