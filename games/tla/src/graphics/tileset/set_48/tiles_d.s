	.section .rodata
	.global Tileset_Set48TilesD
Tileset_Set48TilesD:
	.incbin "games/tla/src/graphics/tileset/set_48/tiles_d.4bpp.mtf"
	.byte 0x50, 0x4e	@ PACKER: left in the buffer by Tileset_Set48TilesC+0xd76
