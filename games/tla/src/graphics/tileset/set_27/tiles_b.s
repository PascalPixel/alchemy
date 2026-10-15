	.section .rodata
	.global Tileset_Set27TilesB
Tileset_Set27TilesB:
	.incbin "games/tla/src/graphics/tileset/set_27/tiles_b.4bpp.mtf"
	.byte 0x11, 0xa7	@ PACKER: left in the buffer by Tileset_Set27TilesA+0x1c52
