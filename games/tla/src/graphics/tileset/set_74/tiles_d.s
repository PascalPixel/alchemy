	.section .rodata
	.global Tileset_Set74TilesD
Tileset_Set74TilesD:
	.incbin "games/tla/src/graphics/tileset/set_74/tiles_d.4bpp.mtf"
	.byte 0xb1, 0x74, 0x4f	@ PACKER: left in the buffer by Tileset_Set74TilesB+0x1349
