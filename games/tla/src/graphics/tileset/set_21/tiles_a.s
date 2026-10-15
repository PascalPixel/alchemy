	.section .rodata
	.global Tileset_Set21TilesA
Tileset_Set21TilesA:
	.incbin "games/tla/src/graphics/tileset/set_21/tiles_a.4bpp.mtf"
	.byte 0x7e, 0xa7	@ PACKER: left in the buffer by Tileset_Set17TilesC+0x1cbe
