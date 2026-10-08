	.section .rodata
	.global Tileset_Set37TilesB
Tileset_Set37TilesB:
	.incbin "GRAPHICS/TILESET/SET_37/TILES_B.4bpp.mtf"
	.byte 0x2f	@ PACKER: left in the buffer by Tileset_Set37TilesA+0x13f
