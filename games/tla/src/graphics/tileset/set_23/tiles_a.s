	.section .rodata
	.global Tileset_Set23TilesA
Tileset_Set23TilesA:
	.incbin "games/tla/src/graphics/tileset/set_23/tiles_a.4bpp.mtf"
	.byte 0x9e	@ PACKER: left in the buffer by Field_Map040+0x1bdf
