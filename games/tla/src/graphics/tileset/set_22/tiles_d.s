	.section .rodata
	.global Tileset_Set22TilesD
Tileset_Set22TilesD:
	.incbin "games/tla/src/graphics/tileset/set_22/tiles_d.4bpp.mtf"
	.byte 0x03, 0x86, 0xc0	@ PACKER: left in the buffer by Tileset_Set22TilesB+0x2151
