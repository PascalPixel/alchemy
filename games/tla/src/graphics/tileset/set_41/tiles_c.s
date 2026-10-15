	.section .rodata
	.global Tileset_Set41TilesC
Tileset_Set41TilesC:
	.incbin "games/tla/src/graphics/tileset/set_41/tiles_c.4bpp.mtf"
	.byte 0xca	@ PACKER: left in the buffer by Tileset_Set41TilesB+0x13f
