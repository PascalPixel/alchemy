	.section .rodata
	.global Tileset_Set27TilesC
Tileset_Set27TilesC:
	.incbin "GRAPHICS/TILESET/SET_27/TILES_C.4bpp.mtf"
	.byte 0xb0, 0x3e	@ PACKER: left in the buffer by Tileset_Set27TilesB+0x197a
