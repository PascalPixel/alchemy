	.section .rodata
	.global Tileset_Set58TilesD
Tileset_Set58TilesD:
	.incbin "games/tla/src/graphics/tileset/set_58/tiles_d.4bpp.mtf"
	.byte 0xdc	@ PACKER: left in the buffer by Tileset_Set58TilesC+0x13f
