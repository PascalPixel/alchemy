	.section .rodata
	.global Tileset_Set03TilesD
Tileset_Set03TilesD:
	.incbin "GRAPHICS/TILESET/SET_03/TILES_D.4bpp.mtf"
	.byte 0xe1, 0xf0, 0xf8	@ PACKER: left in the buffer by Tileset_Set03TilesC+0x1145
