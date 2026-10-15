	.section .rodata
	.global Tileset_Set66TilesD
Tileset_Set66TilesD:
	.incbin "games/tla/src/graphics/tileset/set_66/tiles_d.4bpp.mtf"
	.byte 0xc1, 0x26, 0x82	@ PACKER: left in the buffer by Tileset_Set66TilesA+0x1969
