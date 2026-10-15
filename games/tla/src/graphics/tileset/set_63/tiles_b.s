	.section .rodata
	.global Tileset_Set63TilesB
Tileset_Set63TilesB:
	.incbin "games/tla/src/graphics/tileset/set_63/tiles_b.4bpp.mtf"
	.byte 0x0c	@ PACKER: left in the buffer by Field_Map131+0x1aef
