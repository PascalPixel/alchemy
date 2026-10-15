	.section .rodata
	.global Tileset_Set97TilesA
Tileset_Set97TilesA:
	.incbin "games/tla/src/graphics/tileset/set_97/tiles_a.4bpp.mtf"
	.byte 0xf6, 0xf1	@ PACKER: left in the buffer by Field_Map221+0x2466
