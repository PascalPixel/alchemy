	.section .rodata
	.global Tileset_Set27TilesD
Tileset_Set27TilesD:
	.incbin "games/tla/src/graphics/tileset/set_27/tiles_d.4bpp.mtf"
	.byte 0x50	@ PACKER: left in the buffer by Tileset_Set27TilesC+0x13f
