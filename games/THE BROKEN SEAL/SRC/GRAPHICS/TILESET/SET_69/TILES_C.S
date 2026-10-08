	.section .rodata
	.global Tileset_Set69TilesC
Tileset_Set69TilesC:
	.incbin "GRAPHICS/TILESET/SET_68/TILES_B.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set69TilesA+0x13f
