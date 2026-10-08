	.section .rodata
	.global Tileset_Set28TilesC
Tileset_Set28TilesC:
	.incbin "GRAPHICS/TILESET/SET_28/TILES_C.4bpp.mtf"
	.byte 0x10, 0x84, 0x55	@ PACKER: left in the buffer by Tileset_Set27TilesB+0x22d5
