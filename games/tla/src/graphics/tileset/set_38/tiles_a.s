	.section .rodata
	.global Tileset_Set38TilesA
Tileset_Set38TilesA:
	.incbin "games/tla/src/graphics/tileset/set_38/tiles_a.4bpp.mtf"
	.byte 0x21, 0x0d	@ PACKER: left in the buffer by Field_Map074+0x1a06
