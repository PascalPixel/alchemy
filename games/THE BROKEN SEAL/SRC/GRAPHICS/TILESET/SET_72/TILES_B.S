	.section .rodata
	.global Tileset_Set72TilesB
Tileset_Set72TilesB:
	.incbin "GRAPHICS/TILESET/SET_68/TILES_B.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set72TilesA+0x13f
