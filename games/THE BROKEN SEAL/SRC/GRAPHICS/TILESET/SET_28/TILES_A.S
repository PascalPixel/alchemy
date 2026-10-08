	.section .rodata
	.global Tileset_Set28TilesA
Tileset_Set28TilesA:
	.incbin "GRAPHICS/TILESET/SET_28/TILES_A.4bpp.mtf"
	.byte 0xfe	@ PACKER: left in the buffer by Tileset_Set27TilesB+0x209f
