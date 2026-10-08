	.section .rodata
	.global Tileset_Set10TilesD
Tileset_Set10TilesD:
	.incbin "GRAPHICS/TILESET/SET_10/TILES_D.4bpp.mtf"
	.byte 0xe3, 0xb0, 0xd3	@ PACKER: left in the buffer by Tileset_Set10TilesC+0x225
