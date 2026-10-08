	.section .rodata
	.global Tileset_Set27TilesC
Tileset_Set27TilesC:
	.incbin "graphics/tileset/set_27/tiles_c.4bpp.mtf"
	.byte 0xb0, 0x3e	@ PACKER: left in the buffer by Tileset_Set27TilesB+0x197a
