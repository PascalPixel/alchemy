	.section .rodata
	.global Tileset_Set57TilesB
Tileset_Set57TilesB:
	.incbin "GRAPHICS/TILESET/SET_57/TILES_B.4bpp.mtf"
	.byte 0x21, 0xcf, 0x49	@ PACKER: left in the buffer by Tileset_Set56TilesC+0x1795
