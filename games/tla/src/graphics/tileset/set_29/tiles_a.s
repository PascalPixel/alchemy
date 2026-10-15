	.section .rodata
	.global Tileset_Set29TilesA
Tileset_Set29TilesA:
	.incbin "games/tla/src/graphics/tileset/set_29/tiles_a.4bpp.mtf"
	.byte 0x3e	@ PACKER: left in the buffer by Field_Map040+0x2e4f
