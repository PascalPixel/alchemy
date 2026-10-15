	.section .rodata
	.global Tileset_Set30TilesD
Tileset_Set30TilesD:
	.incbin "games/tla/src/graphics/tileset/set_30/tiles_d.4bpp.mtf"
	.byte 0x53, 0xb6, 0x22	@ PACKER: left in the buffer by Tileset_Set30TilesB+0x1921
