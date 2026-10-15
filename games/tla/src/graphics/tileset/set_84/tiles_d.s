	.section .rodata
	.global Tileset_Set84TilesD
Tileset_Set84TilesD:
	.incbin "games/tla/src/graphics/tileset/set_84/tiles_d.4bpp.mtf"
	.byte 0xbf, 0x45, 0xca	@ PACKER: left in the buffer by Tileset_Set84TilesB+0x1169
