	.section .rodata
	.global Tileset_Set27TilesC
Tileset_Set27TilesC:
	.incbin "games/tla/src/graphics/tileset/set_27/tiles_c.4bpp.mtf"
	.byte 0x43	@ PACKER: left in the buffer by Tileset_Set27TilesB+0x643
