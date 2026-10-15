	.section .rodata
	.global Tileset_Set06TilesD
Tileset_Set06TilesD:
	.incbin "games/tla/src/graphics/tileset/set_06/tiles_d.4bpp.mtf"
	.byte 0x67, 0x14	@ PACKER: left in the buffer by Tileset_Set06TilesC+0x14d2
